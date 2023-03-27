#include "LinkedList.h"

template<class T>
LinkedList<T>::LinkedList()
{
    /* TODO */
    head = NULL;
    size = 0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj)
{
    /* TODO */
    if(this == &obj) return;
    head=NULL;
    size=0;
    Node<T> *curr = obj.head;
    for (int i=0; i<obj.size; i++){
        this->append(curr->data);
        curr = curr->next;
    }
    size = obj.size;
} 

template<class T>
LinkedList<T>::~LinkedList()
{
    /* TODO */ 
    this->removeAllNodes();
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    /* TODO */ 
    if(this == &rhs) return *this;
    removeAllNodes();
    Node<T> *curr = rhs.head;
    for (int i=0; i<rhs.size; i++){
        this->append(curr->data);
        curr = curr->next;
    }
    size = rhs.size;
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const
{
    /* TODO */
    return size;
    
}

template<class T>
bool LinkedList<T>::isEmpty() const
{
    /* TODO */
    if (head == NULL) return true;
    return false;
}

template<class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    /* TODO */
    if (this->isEmpty()) return false;
    if (head == node) return true;
    Node<T> *curr = head->next;
    while (curr != head){
        if (curr == node) return true;
        curr = curr->next;
    }
    return false;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    /* TODO */
    return head;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    /* TODO */
    if (head){ 
        return head->prev;
    }
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{
    /* TODO */
    if (this->isEmpty()) return NULL;
    Node<T> *curr = head;
    for (int i=0; i<size; i++){
        if (curr->data == data) return curr;
        curr = curr->next;
    }
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    /* TODO */
    if (this->isEmpty()) return NULL;
    if (index > size-1 || index < 0) return NULL;
    Node<T> *curr = head;
    for (int i=0; i<index; i++){
        curr = curr->next;
    }
    return curr;
    
}

template<class T>
void LinkedList<T>::append(const T &data)
{
    /* TODO */ 
    if (head){
        Node<T> *last = head->prev;
        head->prev = new Node<T>(data, last, head);
        last->next = head->prev;
    }
    else {
        head = new Node<T>(data);
        head->next = head;
        head->prev = head;
    }
    size++;
}

template<class T>
void LinkedList<T>::prepend(const T &data)
{
    /* TODO */
    Node<T> *curr = head;
    if (curr) {
        Node<T> *last = head->prev;
        head = new Node<T>(data, last, curr);
        last->next = head;
        curr->prev = head;
    }
    else {
        head = new Node<T>(data);
        head->next = head;
        head->prev = head;
    }
    size++;
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    /* TODO */ 
    if (!this->containsNode(node)) return;
    Node<T> *after = node->next;
    node->next = new Node<T>(data, node, after);
    after->prev = node->next;
    size++;
}

template<class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{ 
    /* TODO */
    if (index > size || index < 0) return;
    Node<T> *boom; 
    if (size == 0){
        head = new Node<T>(data);
        head->next = head->prev = head;
    }
    else if (index == 0){
        head = new Node<T>(data, head->prev, head->next);
        head->prev->next = head;
        head->next->prev = head;
    }
    else {
        boom = (this->getNodeAtIndex(index))->prev;
        this->insertAfterNode(data, boom);
        size--;
    }
    size++;
}

template<class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    /* TODO */
    if (!this->containsNode(node)) return -1;
    Node<T> *curr = head;
    int index=0;
    for (int i=0; i<size; i++){
        if (curr == node) return index;
        curr = curr->next;
        index++;
    }
}

template<class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{
    /* TODO */
    if (this->isEmpty() || currentIndex > size-1 || currentIndex < 0 || newIndex < 0) return;
    if (newIndex > size-1) newIndex = size-1;
    if (currentIndex == newIndex) return;
    Node<T> *boom = this->getNodeAtIndex(currentIndex);
    if (currentIndex == 0){
        Node<T> *newhead = head->next;
        Node<T> *curr = head->next;
        Node<T> *prev = boom->prev;
        head = newhead;
        prev->next = boom->next;
        boom->next->prev = prev;
        for (int i=0; i<newIndex; i++){
            curr = curr->next;
        }
        curr->prev->next = boom;
        boom->next = curr;
        boom->prev = curr->prev;
        curr->prev = boom;
    }

    else if (newIndex == 0 && currentIndex == size-1){
        head = head->prev;
    }

    else {
        Node<T> *curr = head;
        Node<T> *prev = boom->prev;
        prev->next = boom->next;
        boom->next->prev = prev;
        for (int i=0; i<newIndex; i++){
            curr = curr->next;
        }
        curr->prev->next = boom;
        boom->next = curr;
        boom->prev = curr->prev;
        curr->prev = boom;
    }
    if (newIndex == 0) this->head = boom;
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    /* TODO */
    if (!this->containsNode(node)) return;
    if (head->next == head){
        head = NULL;
    }
    else {
        if (head == node){
            head->next->prev = head->prev;
            head->prev->next = head->next;
            head = head->next;
        }
        else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
    }
    delete node;
    size -= 1;
}

template<class T>
void LinkedList<T>::removeNode(const T &data)
{
    /* TODO */
    Node<T> *boom = this->getNode(data);
    while (boom){
        this->removeNode(boom);
        boom = this->getNode(data);
    }
}

template<class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    /* TODO */
    if (index > size-1 || index < 0) return;
    Node<T> *boom = this->getNodeAtIndex(index);
    removeNode(boom);
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    /* TODO */
    Node<T> *curr = head;
    while(curr){
        this->removeNode(curr);
        curr = head;
    }
}

template<class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    do {
        std::cout << *node << std::endl;
        node = node->next;
    }
    while (node != this->getFirstNode());
}
