#include "BinaryHeap.h"

BinaryHeap::BinaryHeap()
{
    // TODO: or not
    elements.resize(1);
    size = 0;
}


bool BinaryHeap::Add(int uniqueId, double weight)
{
    // TODO:
    if (!this->contains(uniqueId)){
        if(size+1 == elements.size()){
            elements.resize(elements.size()*2 +1);
        }
        size++;
        elements[size].uniqueId = uniqueId;
        elements[size].weight = weight;
        for(int i = size/2; i > 0; i-- ){
            perdown(i);
        }
        return true;
    }
    return false;
}

bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    // TODO:
    if (size == 0) return false;
    else{
        outUniqueId = elements[1].uniqueId;
        outWeight = elements[1].weight;
        elements[1].uniqueId = elements[size].uniqueId;
        elements[1].weight = elements[size].weight;
        size--;
        perdown(1);
        return true;
    }
    
}

void BinaryHeap::perdown(int index){
    if (size==0) return;
    int child;
    int tempId = elements[index].uniqueId;
    double tempWeight = elements[index].weight;
    while (index*2 <= size){
        child = index*2;
        if (child != size && elements[child+1].weight < elements[child].weight){
            child++;
        }
        if (elements[child].weight < tempWeight){
            elements[index].weight = elements[child].weight;
            elements[index].uniqueId = elements[child].uniqueId;
        }
        else break;
        index = child;
    }
    elements[index].uniqueId = tempId;
    elements[index].weight = tempWeight;
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    // TODO:
    if(!this->contains(uniqueId)) return false;
    else{
        int index;
        for (int i = 1; i<=size; i++){
            if (elements[i].uniqueId == uniqueId){
                index = i;
                break;
            }
        }
        elements[index].weight = newWeight;
        for(int i = size/2; i > 0; i-- ){
            perdown(i);
        }
        return true;
    }

}

int BinaryHeap::HeapSize() const
{
    // TODO:
    return size;
}

bool BinaryHeap::contains(int uniqueId){
    if (size == 0) return false;
    else{
        for (int i = 1; i<=size; i++){
            if (elements[i].uniqueId == uniqueId) return true;
        }
    }
    return false;
}







