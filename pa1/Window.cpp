#include "Window.h"

#include "Window.h"

Window::Window() {
    this->activeTab = 0;
    tabs.append(Tab());
}

Tab Window::getActiveTab() {
    // TODO
    if (this->isEmpty()) return Tab();
    return tabs.getNodeAtIndex(activeTab)->data;
}

bool Window::isEmpty() const {
    // TODO
    return tabs.isEmpty();
}

void Window::newTab(const Tab &tab) {
    // TODO
    if (this->isEmpty()){
        activeTab = 0;
        tabs.append(tab);
    }
    else {
        tabs.insertAfterNode(tab, tabs.getNodeAtIndex(activeTab));
        activeTab++;
    }
}

void Window::closeTab() {
    // TODO
    if (this->isEmpty()) return;
    tabs.removeNode(tabs.getNodeAtIndex(activeTab));
    if (tabs.getSize() == 0) activeTab = -1;
    else if (activeTab == 0) return;
    else if(tabs.getSize() ==activeTab) activeTab--;
}

void Window::moveActiveTabTo(int index) {
    // TODO
    if (this->isEmpty() || index < 0) return;
    tabs.moveToIndex(activeTab, index);
    if (index > tabs.getSize()-1) index = tabs.getSize()-1;
    activeTab = index;
}

void Window::changeActiveTabTo(int index) {
    // TODO
    if (this->isEmpty() || index < 0 || index > tabs.getSize()-1 ) return;
    activeTab = index;
}

void Window::addTab(Node<Tab> &tab) {
    // TODO
    if (this->isEmpty()) {
        tabs.append(tab.data);
        activeTab = 0;
    }
    else {
        tabs.append(tab.data);
    }
}

void Window::print() {
    if(tabs.isEmpty()) std::cout << "Window is empty" << std::endl;
    else std::cout << "The active tab is tab " << activeTab << std::endl;
    tabs.print();
}