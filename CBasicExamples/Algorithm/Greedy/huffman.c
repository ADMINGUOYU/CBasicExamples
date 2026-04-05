// Greedy Algorithm
// Huffman Coding Problem

/*
We got a table of n characters C1, C2, ..., Cn and their frequencies f1, f2, ..., fn.
We want to encode these characters using binary strings. 
We want to assign a binary string Si to each character Ci such that no Si is a prefix
of any other Sj (i != j). 
The length of the binary string Si is denoted by li. The cost of the encoding is defined
as sum of fi * li for i = 1, 2, ..., n.

-> We implement a huffman tree here
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Character and Frequency Pair
typedef struct char_freq_pair {
    char character; // character
    int frequency;  // frequency
} char_freq_pair;

// Huffman Tree Node
typedef struct huffman_node {
    char_freq_pair char_freq;   // character and frequency pair
    struct huffman_node* left;  // left child
    struct huffman_node* right; // right child
} huffman_node;

// Min-Heap for Huffman Nodes
typedef struct min_heap {
    int size;             // current size of the heap
    int capacity;         // maximum capacity of the heap
    huffman_node** array; // array of pointers to huffman nodes
} min_heap;

// Character and Code Pair
typedef struct character_code {
    char character; // character
    char* code;     // binary code for the character
} character_code;

// Result mapping table
typedef struct result_mapping_table {
    int size;                 // number of character-code pairs
    character_code* char_code; // array of character-code pairs
    int total_cost;           // total cost of the encoding (sum of fi * li)
} result_mapping_table;

// ================================
// Function prototypes
// ================================
// Min-Heap functions
min_heap* create_min_heap(int capacity);
void insert_min_heap(min_heap* heap, huffman_node* node);
huffman_node* extract_min(min_heap* heap);
// Huffman Tree functions
huffman_node* create_huffman_node(char character, int frequency);
void build_huffman_tree(char_freq_pair* char_freqs, int n, huffman_node** root);
void generate_codes(huffman_node* root, char* code, int depth, result_mapping_table* mapping_table);
void free_huffman_tree(huffman_node* root);
void free_result_mapping_table(result_mapping_table* mapping_table);
// Function to print the character-code pairs
void print_character_codes(result_mapping_table* mapping_table);

// ================================
// Utility function definitions
// ================================
// Min-Heap functions
min_heap* create_min_heap(int capacity)
{
    // Allocate memory for the min-heap structure
    min_heap* heap = (min_heap*) malloc (sizeof(min_heap));

    // Initialize the size and capacity of the heap
    heap->size = 0;
    heap->capacity = capacity;

    // Allocate memory for the array of pointers to huffman nodes
    heap->array = (huffman_node**) malloc (capacity * sizeof(huffman_node*));

    return heap;
}
void insert_min_heap(min_heap* heap, huffman_node* node)
{
    // Insert the node into the min-heap and maintain the heap property

    // Check if the heap is full
    if (heap->size == heap->capacity)
    {
        printf("\033[1;31mMin-Heap is full. Cannot insert new node.\033[0m\n");
        return;
    }

    // Insert the node at the end of the array
    heap->array[heap->size] = node;
    ++(heap->size);

    // Maintain the min-heap property by bubbling up the new node
    int i = heap->size - 1;    // Index of the newly inserted node
    while (i > 0) {
        // Calculate the index of the parent node
        int parent = (i - 1) / 2;
        // Compare the frequency of the current node with its parent
        if (heap->array[i]->char_freq.frequency < heap->array[parent]->char_freq.frequency) 
        {
            // Swap the nodes
            huffman_node* temp = heap->array[i];
            heap->array[i] = heap->array[parent];
            heap->array[parent] = temp;
            // Move up to the parent index
            i = parent;
        } 
        else
            break; // The heap property is satisfied
    }

    // DONE
    return;
}
huffman_node* extract_min(min_heap* heap)
{
    // Extract the node with the minimum frequency from the min-heap and maintain the heap property

    // Check if the heap is empty
    if (heap->size == 0)
    {
        printf("\033[1;31mMin-Heap is empty. Cannot extract node.\033[0m\n");
        return NULL;
    }

    // Store the minimum node (the root of the heap)
    huffman_node* min_node = heap->array[0];

    // Move the last node to the root and decrease the size of the heap
    heap->array[0] = heap->array[heap->size - 1];
    --(heap->size);

    // Maintain the min-heap property by bubbling down the new root node
    int i = 0; // Index of the current node
    while (1) {
        int left = 2 * i + 1;   // Index of left child
        int right = 2 * i + 2;  // Index of right child
        int smallest = i;        // Assume current node is smallest

        // Compare with left child
        if (left < heap->size && 
            heap->array[left]->char_freq.frequency < heap->array[smallest]->char_freq.frequency)
            smallest = left;

        // Compare with right child
        if (right < heap->size && 
            heap->array[right]->char_freq.frequency < heap->array[smallest]->char_freq.frequency)
            smallest = right;

        // If the smallest is not the current node, swap and continue bubbling down
        if (smallest != i) 
        {
            huffman_node* temp = heap->array[i];
            heap->array[i] = heap->array[smallest];
            heap->array[smallest] = temp;
            // Move down to the smallest index
            i = smallest;
        }
        else
            break; // The heap property is satisfied
    }

    // Return the extracted minimum node
    return min_node;
}
// Huffman Tree functions
huffman_node* create_huffman_node(char character, int frequency)
{
    // Allocate memory for a new huffman node
    huffman_node* node = (huffman_node*) malloc (sizeof(huffman_node));

    // Initialize the character and frequency of the node
    node->char_freq.character = character;
    node->char_freq.frequency = frequency;

    // Initialize the left and right children to NULL
    node->left = NULL;
    node->right = NULL;

    return node;
}
void build_huffman_tree(char_freq_pair* char_freqs, int n, huffman_node** root)
{
    // Build the huffman tree using the given character-frequency pairs

    // Create a min-heap to store the huffman nodes
    min_heap* heap = create_min_heap(n);

    // Insert all characters and their frequencies into the min-heap as huffman nodes
    for (int i = 0; i < n; ++i)
    {
        huffman_node* node = create_huffman_node(char_freqs[i].character, char_freqs[i].frequency);
        insert_min_heap(heap, node);
    }

    // if there is only one character, we create a dummy parent node to ensure the huffman tree has at least two nodes,
    // which allows us to generate a valid code for the single character
    if (heap->size == 1)
    {
        huffman_node* single_node = extract_min(heap);
        huffman_node* dummy_node = create_huffman_node('\0', single_node->char_freq.frequency); // Create a dummy node with the same frequency
        dummy_node->left = single_node; // Make the single node the left child of the dummy node
        insert_min_heap(heap, dummy_node); // Insert the dummy node back into the heap
    }

    // Build the huffman tree until there is only one node left in the min-heap
    while (heap->size > 1)
    {
        // Extract the two nodes with the smallest frequencies
        huffman_node* left = extract_min(heap);
        huffman_node* right = extract_min(heap);

        // Create a new internal node with these two nodes as children
        // The frequency of the new node is the sum of the frequencies of the two nodes
        // NOTE: Use '\0' as the character for internal nodes since they do not represent actual characters
        huffman_node* internal_node = create_huffman_node('\0', left->char_freq.frequency + right->char_freq.frequency);
        internal_node->left = left;
        internal_node->right = right;
        insert_min_heap(heap, internal_node);
    }

    // The last node in the heap is the root of the Huffman tree
    *root = extract_min(heap);
    // Now the heap is empty, we can free the heap memory

    // Free the min-heap memory
    free(heap->array);
    free(heap);

    // DONE
    return;
}
void generate_codes(huffman_node* root, char* code, int depth, result_mapping_table* mapping_table)
{
    // Generate the binary codes for each character by traversing the huffman tree
    // NOTE: result mapping table should have enough capacity to store all character-code pairs (size >= number of characters)

    /* 
    Arguments:
    - root: current node in the huffman tree
    - code: array to store the current binary code being generated (should have enough capacity to store the longest code)
            buffer size estimation: maximum depth of the huffman tree (which is at most n for n characters) + 1 for null terminator
    - depth: current depth in the huffman tree (also represents the length of the current code)
    - mapping_table: pointer to the result mapping table to store the character-code pairs
    */

    // Base case: if the current node is NULL, return
    if (root == NULL)
        return;

    // If the current node is a leaf node (i.e., it represents a character), store the character and its code in the mapping table
    if (root->left == NULL && root->right == NULL)
    {
        // Store the character and its code in the mapping table
        mapping_table->char_code[mapping_table->size].character = root->char_freq.character;
        code[depth] = '\0'; // Null-terminate the current code
        mapping_table->char_code[mapping_table->size].code = (char*) malloc ((depth + 1) * sizeof(char)); // Allocate memory for the code string
        for (int i = 0; i <= depth; ++i)
            mapping_table->char_code[mapping_table->size].code[i] = code[i]; // Copy the current code to the mapping table
        // Update the total cost of encoding
        mapping_table->total_cost += root->char_freq.frequency * depth;
        // Increment the size of the mapping table
        ++(mapping_table->size);
    }
    // If this is a parent node, we need to generate the code for its children
    else
    {
        // Generate code for the left child by appending '0' to the current code
        code[depth] = '0';
        generate_codes(root->left, code, depth + 1, mapping_table);

        // Generate code for the right child by appending '1' to the current code
        code[depth] = '1';
        generate_codes(root->right, code, depth + 1, mapping_table);
    }

    // DONE
    return;
}
void free_huffman_tree(huffman_node* root)
{
    // Free the memory allocated for the huffman tree using post-order traversal
    if (root == NULL)
        return;
    
    // Free the left and right subtrees first
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);

    // Free the current node
    free(root);
    root = NULL; // Set the pointer to NULL after freeing

    // DONE
    return;
}
void free_result_mapping_table(result_mapping_table* mapping_table)
{
    // Free the memory allocated for the result mapping table
    for (int i = 0; i < mapping_table->size; ++i)
    {
        free(mapping_table->char_code[i].code);
    }
    free(mapping_table->char_code);
    free(mapping_table);

    // DONE
    return;
}

// Solver function
result_mapping_table* huffman_coding_solver(char_freq_pair* char_freqs, int n)
{
    // Parse the input character-frequency pairs and build the huffman tree
    huffman_node* root = NULL;
    build_huffman_tree(char_freqs, n, &root);

    // Initialize the result mapping table
    result_mapping_table* mapping_table = (result_mapping_table*) malloc (sizeof(result_mapping_table));
    mapping_table->size = 0;
    mapping_table->char_code = (character_code*) malloc (n * sizeof(character_code));
    mapping_table->total_cost = 0;

    // Generate the binary codes for each character by traversing the huffman tree
    char* code_buffer = (char*) malloc ((n + 1) * sizeof(char)); // Buffer to store the current code being generated
    generate_codes(root, code_buffer, 0, mapping_table);
    // Free the code buffer memory
    free(code_buffer);

    // Free the huffman tree memory
    free_huffman_tree(root);

    // Return the result mapping table
    return mapping_table;
}

// ================================
// Entry point
// ================================
int alg_greedy_huffman_main(int argc, char* argv[])
{
    // Initialize characters and frequencies
    char_freq_pair char_freqs_default[] = {
        {'a', 77},
        {'b', 17},
        {'c', 32},
        {'d', 42},
        {'e', 120},
        {'f', 24},
        {'g', 17},
        {'h', 50},
        {'i', 76},
        {'j', 4}
    };

    // Get the number of characters
    int n = sizeof(char_freqs_default) / sizeof(char_freqs_default[0]);

    // Prompt the user to input character-frequency pairs
    printf("\033[1;34m>>> Huffman Coding [INPUT] >>>\033[0m\n");
    printf("Do you want to use the default character-frequency pairs? (y/n): ");
    char choice;
    scanf(" %c", &choice); //  Adding a space in the format string tells scanf to discard all consecutive whitespace until it finds the first non-whitespace character. 

    // Define a pointer to the character-frequency pairs
    char_freq_pair* char_freqs = NULL;
    
    // Parse user input
    if (choice == 'y' || choice == 'Y') {
        char_freqs = char_freqs_default;
    } else {
        printf("Enter the number of character-frequency pairs: ");
        scanf("%d", &n);
        char_freqs = (char_freq_pair*) malloc (n * sizeof(char_freq_pair));
        for (int i = 0; i < n; ++i) {
            printf("Enter character and frequency for pair %d (format: char frequency): ", i + 1);
            scanf(" %c %d", &(char_freqs[i].character), &(char_freqs[i].frequency));
        }
    }
    while (getchar() != '\n'); // Clear the input buffer to remove any leftover characters, including the newline character

    // Get the result mapping table
    result_mapping_table* mapping_table = huffman_coding_solver(char_freqs, n);

    // Print the result
    printf("\033[1;32m>>> Huffman Coding [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mInput pairs:\033[0m\n");
    for (int i = 0; i < n; ++i) {
        if (i < n - 1) printf("  │   ├── ");
        else printf("  │   └── ");
        printf("char(%c): freq(%d)\n", char_freqs[i].character, char_freqs[i].frequency);
    }
    printf("  ├── \033[1mTotal cost of encoding:\033[0m %d\n", mapping_table->total_cost);
    // Print the character-code pairs
    printf("  └── \033[1mCharacter-Code Pairs:\033[0m\n");
    for (int i = 0; i < mapping_table->size; ++i) {
        if (i < mapping_table->size - 1) printf("      ├── ");
        else printf("      └── ");
        printf("char(%c): code(%s)\n", mapping_table->char_code[i].character, mapping_table->char_code[i].code);
    }

    // Free the allocated memory for the result mapping table
    free_result_mapping_table(mapping_table);

    // Free the allocated memory for the character-frequency pairs if we used user input
    if (choice != 'y' && choice != 'Y') free(char_freqs);

    return 0;
}