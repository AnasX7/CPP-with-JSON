#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "json.hpp"
using namespace nlohmann;
using namespace std;

struct Node {
    int id;
    string info;
    string date;
    Node* next;
};
Node* head = NULL;

string getDate();
void addTodo(string todo_info);
void removeTodo(int todo_id);
void updateId();
void displayTodoList();
void updateJsonFile();
void updateList();

int main() {
    updateList();
    removeTodo(8);
    updateJsonFile();
    displayTodoList();
}

string getDate() {
    auto now = chrono::system_clock::now();
    time_t current_time = chrono::system_clock::to_time_t(now);
    tm* timeinfo = localtime(&current_time);

    string date = to_string(timeinfo->tm_mday) + "/"
                  + (timeinfo->tm_mon < 9 ? "0" : "") 
                  + to_string(1 + timeinfo->tm_mon) 
                  + "/" + to_string(1900 + timeinfo->tm_year);

    return date;                  
}

void addTodo(string todo_info) {
    Node* newTodo = new Node;
    newTodo->info = todo_info;
    newTodo->date = getDate();

    int todo_id = 1;

    if (head == NULL) {
        head = newTodo;
        newTodo->next = NULL;
        newTodo->id = todo_id;
    } else {
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
            todo_id++;
        }
        current->next = newTodo;
        newTodo->next = NULL;
        newTodo->id = todo_id + 1;
    }
}

void removeTodo(int todo_id) {
    if (head == NULL) {
        cout << "There is No Todo in the List!\n";
    } else {
        Node* current = head;
        Node* prev = head;
        while (current != NULL && current->id != todo_id) {
            prev = current;
            current = current->next;
        }
        if (current == NULL) {
            cout << "The Todo not found in the List!\n";
            return;
        }

        prev->next = current->next;
        delete current;
        current = NULL;

        updateId();
    }
}

void updateId() {
    if (head != NULL) {
        Node* current = head;
        int todo_id = 1;
        while (current != NULL) {
            current->id = todo_id;
            current = current->next;
            todo_id++;
        }
    }
}

void displayTodoList() {
    if (head == NULL) {
        cout << "There is No Todo in the List!\n";
    } else {
        Node* current = head;
        while (current != NULL) {
            cout << "ID: " << current->id << "\n";
            cout << "Todo: " << current->info << "\n";
            cout << "Date: " << current->date << "\n";
            cout << "----------------------------------\n";
            current = current->next;
        }
    }
}

void updateJsonFile() {
    ofstream todo_DB("./toDoListDB.json");
    json newJson;
    newJson["ToDoList"] = json::array();

    if (head == NULL) {
        todo_DB << newJson;
    } else {
        Node* current = head; 
        while (current != NULL) {
            newJson["ToDoList"].push_back({
                {"ID", current->id}, 
                {"ToDo", current->info}, 
                {"Date", current->date}
            });
            current = current->next;
        }
        todo_DB << setw(4) << newJson;
    }
}

void updateList() {
    ifstream jsonFile("./toDoListDB.json");
    json jsonData = json::parse(jsonFile);
    
    auto todoList = jsonData["ToDoList"];

    for (const auto& todo : todoList) {
        Node* newTodo = new Node;
        newTodo->id = todo["ID"];
        newTodo->info = todo["ToDo"];
        newTodo->date = todo["Date"];

        if (head == NULL) {
            head = newTodo;
            newTodo->next = NULL;
        } else {
            Node* current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newTodo;
            newTodo->next = NULL;
        }
    }
}
