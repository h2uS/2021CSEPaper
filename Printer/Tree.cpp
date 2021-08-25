#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct Node
{
    int value;
    // -1 : not leaf node
    // 0 : Re
    // 1, 2, ... : Event number (Ev_i)
    vector<Node*> children;
};

class TrialRule
{
private:
    Node* root;
    int k;

    void delete_node(Node* node)
    {
        if (!node->children.empty()) {
            for (int i = 0; i < k; i++) {
                Node* child = node->children.back();
                node->children.pop_back();
                delete_node(child);
            }
        }
        delete node;
    }

    void print_node(Node* node, vector<int> path, ofstream& ofs)
    {
        if (node->value != -1) {
            for (int& edge : path) {
                ofs << edge << " ";
            }

            if (node->value == 0) {
                ofs << " - Re" << endl;
            }
            else {
                ofs << " - E_" << node->value << endl;
            }
        }
        else { // not leaf node
            if (!path.empty()) {
                for (int& edge : path) {
                    ofs << edge << " ";
                }

                ofs << endl;
            }

            for (int i = 0; i < k; i++) {
                vector<int> new_path;
                for (int& edge : path) {
                    new_path.push_back(edge);
                }
                new_path.push_back(i+1);

                print_node(node->children[i], new_path, ofs);
            }
        }
    }

public:
    TrialRule(int k, int height, vector<long long> v, ofstream& ofs) : k(k)
    {
        long long num_leaf = 1;
        for (int i = 0; i < height; i++) {
            num_leaf *= k;
        }

        long long v_sum = 0;
        for (long long& n : v) {
            v_sum += n;
        }

        v.insert(v.begin(), num_leaf - v_sum); // v[0] : # of Re, v[i] (i > 0) : # of E_i 

        vector<vector<int>> k_nums;
        for (long long& n : v) {
            vector<int> k_nums_ele(height, 0);
            long long d = n;
            for (int i = height - 1; d > 0; i--) {
                k_nums_ele[i] = d % k;
                d /= k;
            }
            k_nums.push_back(k_nums_ele);
        }

        // debug
        //for (int i = 0; i < k_nums.size(); i++) {
        //    if (i == 0) {
        //        ofs << "Re  \t: ";
        //    }
        //    else {
        //        ofs << "E_" << i << "  \t: ";
        //    }
        //    for (int n : k_nums[i]) {
        //        ofs << n << " ";
        //    }
        //    ofs << endl;
        //}
        //ofs << endl;
        ////

        root = new Node;
        deque<Node*> currents;
        currents.push_back(root);

        for (int i = 0; i < height; i++) {
            int cur_size = currents.size();
            for (int j = 0; j < cur_size; j++) {
                currents[j]->value = -1;
                for (int l = 0; l < k; l++) {
                    Node* new_node = new Node;
                    currents[j]->children.push_back(new_node);
                    currents.push_back(new_node);
                }
            }
            for (int j = 0; j < cur_size; j++) {
                currents.pop_front();
            }

            for (int j = 0; j < k_nums.size(); j++) {
                for (int l = 0; l < k_nums[j][i]; l++) {
                    currents.front()->value = j;
                    currents.pop_front();
                }
            }
        }
    }
    
    ~TrialRule()
    {
        delete_node(root);
    }

    void Print(ofstream& ofs)
    {
        print_node(root, vector<int>(), ofs);
    }
};