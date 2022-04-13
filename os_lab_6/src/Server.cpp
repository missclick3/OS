#include <bits/stdc++.h>
#include "CalculationNode.hpp"
#include "ZMQFunctions.hpp"
#include "BalancedTree.hpp"

int main() {
    std:: string command;
    CalculationNode node(-1, -1, -1);
    std:: string answer;
    std:: cout << "Menu:\n"
                "\t1. create <ID> -- create a node\n"
                "\t2. exec <ID> <STR1> <STR2> -- search STR2 in STR1\n"
                "\t3. ping <ID> -- check node existence\n"
                "\t3. kill <ID> --  delete a calculation node\n";
    BalancedTree tree;
    while ((std:: cout << "Please enter your command:\n") && (std:: cin >> command)) {
        if (command == "create") {
            int child;
            std:: cin >> child;
            if (tree.Exist(child)) {
                std:: cout << "Error: Already exists" << std:: endl;
            }
            else {
                while (true) {
                    int idParent = tree.FindID();
                    if (idParent == node.id) {
                        answer = node.create(child);
                        tree.AddInTree(child, idParent);
                        break;
                    }
                    else {
                        std:: string message = "create " + std:: to_string(child);
                        answer = node.sendstring(message, idParent);
                        if (answer == "Error: Parent not found") {
                            tree.AvailabilityCheck(idParent);
                        }
                        else {
                            tree.AddInTree(child, idParent);
                            break;
                        }
                    }  
                }
                std:: cout << answer << std::endl;
            }
        }
        else if (command == "exec") {
            std:: string str;
            int child;
            std:: cin >> child;
            getline(std:: cin, str);
            if (!tree.Exist(child)) {
                std:: cout << "Node doesn't exist\n";
            }
            else {
                std:: string message = "exec " + str;
                answer = node.sendstring(message, child);
                std:: cout << answer << std:: endl;
            }
        }
        else if (command == "ping") {
            int child;
            std:: cin >> child;
            if (!tree.Exist(child)) {
                std::cout << "Ok: 0" << std:: endl;
            }
            else if (node.left_id == child || node.right_id == child) {
                answer = node.ping(child);
                std:: cout << answer << std:: endl;
            }
            else {
                std:: string message = "ping " + std:: to_string(child);
                answer = node.sendstring(message, child);
                if (answer == "Error: Parent not found") {
                    answer = "Ok: 0";
                }
                std:: cout << answer << std:: endl;
            }
        }
        else if (command == "kill") {
            int child;
            std:: cin >> child;
            std:: string message = "kill";
            if (!tree.Exist(child)) {
                std:: cout << "Error: Parent is not existed" << std:: endl;
            }
            else {
                answer = node.sendstring(message, child);
                if (answer != "Error: Parent not found") {
                    tree.RemoveFromRoot(child);
                    if (child == node.left_id){
                        unbind(node.left, node.left_port);
                        node.left_id = -2;
                        answer = "Ok";
                    }
                    else if (child == node.right_id) {
                        node.right_id = -2;
                        unbind(node.right, node.right_port);
                        answer = "Ok";
                    }
                    else {
                        message = "clear " + std:: to_string(child);
                        answer = node.sendstring(message, std:: stoi(answer));
                    }
                    std:: cout << answer << std:: endl;
                }
            }
        }
        else {
            std:: cout << "Please enter correct command!" << std:: endl;
        }
    }
    node.kill();
    return 0;
}
