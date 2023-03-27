#include "Browser.h"

Browser::Browser() {
    windows.append(Window());
}

void Browser::newWindow() {
    // TODO
    windows.prepend(Window());
}

void Browser::closeWindow() {
    // TODO
    Node<Window> *head = windows.getFirstNode();
    if (head == NULL) return;
    else {
        while (!(head->data).isEmpty()){
        (head->data).closeTab();
        }
        windows.removeNodeAtIndex(0);
    }
}

void Browser::switchToWindow(int index) {
    // TODO
    if (windows.isEmpty() || index == 0) return;
    windows.moveToIndex(index, 0);
}

Window &Browser::getWindow(int index) {
    return windows.getNodeAtIndex(index) -> data;
}

void Browser::moveTab(Window &from, Window &to) {
    // TODO
    if (from.isEmpty()) return;
    Tab boom = from.getActiveTab();
    from.closeTab();
    to.newTab(boom);
}

void Browser::mergeWindows(Window &window1, Window &window2) {
    // TODO
    if (window2.isEmpty()) return;
    window2.changeActiveTabTo(0);
    while (!window2.isEmpty()){
        window1.newTab(window2.getActiveTab());
        window2.closeTab();
    }
}

void Browser::mergeAllWindows() {
    // TODO
    if (windows.isEmpty()) return;
    Node<Window> *curr = windows.getFirstNode()->next;
    Node<Window> *head = windows.getFirstNode();
    while (curr != head){
        this->mergeWindows(head->data, curr->data);
        curr = curr->next;
    }
}

void Browser::closeAllWindows() {
    // TODO
    if (windows.isEmpty()) return;
    Node<Window> *curr = windows.getFirstNode();
    while (curr){
        this->closeWindow();
        curr = windows.getFirstNode();
    }
}

void Browser::closeEmptyWindows() {
    // TODO
    if (windows.isEmpty()) return;
    Node<Window> *curr = windows.getFirstNode()->next;
    Node<Window> *next = curr;
    while (curr != windows.getFirstNode()){
        next = curr->next;
        if ((curr->data).isEmpty()){
            windows.removeNode(curr);
        }
        curr = next;
    }
    if ((curr->data).isEmpty()) {
        windows.removeNode(curr);
    }
}

void Browser::print() {
    Node<Window> *head = windows.getFirstNode();
    if(head == NULL) {
        std::cout << "The browser is empty" << std::endl;
    } else {
        (head -> data).print();
        head = head -> next;
        while(head != windows.getFirstNode()) {
            (head -> data).print();
            head = head -> next;
        }
    }
}