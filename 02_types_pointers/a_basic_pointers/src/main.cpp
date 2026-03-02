#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include <bitset>

// Generic function that prints the N next integer elements
// void print_elements_ptr(int* ptr, int N)
// void switch_elements_ptr(int *ptr_1, int *ptr_2, int N)
// int* generate_array(int size)
int main() {

    // Three types of data structures
    std::vector<int> data1 = {0, 1, 2, 3, 4, 5, 6, 7}; 
    std::array<int, 8> data2 = {0, 1, 2, 3, 4, 5, 6, 7};
    int data3[] = {0, 1, 2, 3, 4, 5, 6, 7};

    /*
    // Using the function with different data structures
    print_elements_ptr(&data1[2], 3); // Expected output: 2 3 4
    print_elements_ptr(&data2[2], 3); // Expected output: 2 3 4
    print_elements_ptr(&data3[2], 3); // Expected output: 2 3 4
    */

    /*
    // Switching elements between two arrays
    int data4[] = {10, 11, 12, 13};
    switch_elements_ptr(&data1[2], &data4[0], 4);
    print_elements_ptr(&data1[0], data1.size()); // Expected output: 0 1 10 11 12 13 6 7
    */

    /*
    // Demonstrating memory allocation
    int* my_array = generate_array(50);
    print_elements_ptr(my_array+2, 3); // Expected output: 2 3 4
    delete[] my_array;
    */

    // Memory allocation without deallocation (for demonstration purposes)
    /*
    int chunk_size = 30*1024*1024; // 30 million integers ~ 120 MB
    for(int i = 0; i < 30; ++i) {
        int* leak_array = generate_array(chunk_size);
        std::cout<<i<<" / "<<leak_array[50]<<std::endl;
        // Intentionally not deleting leak_array to simulate a memory leak

        // Pause for a short duration
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Uncomment to add delete[] leak_array; here to prevent memory leak
        // delete[] leak_array;
        // leak_array = nullptr;
    

        // Uncomment this line to generate a segmentation fault (if you have deallocated the leak_array and set it to nullptr)
        // leak_array[856] = 45; 

    }
    */
    

    // Bitwise operations on byte data
    std::vector<uint8_t> byte_vector = {0b00001111, 0b10000001, 0b00001010, 0b11110000, 0b10000010};
    // Note: {0b00001111, 0b10000001, 0b00001010, 0b11110000, 0b10000010} = {15, 129, 10, 240, 130} in unsigned integers
    for(uint8_t byte : byte_vector) {

        // To fill ...
        // Byte with first bit 1: 10000001 -> 00000001 (1)
        // Byte with first bit 1: 11110000 -> 01110000 (112)
        // Byte with first bit 1: 10000010 -> 00000010 (2)
    }


    return 0;
}