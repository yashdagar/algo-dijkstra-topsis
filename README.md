# Multi-Attribute Dijkstra with TOPSIS Implementation

This C program implements multi-attribute Dijkstra's algorithm with TOPSIS (Technique for Order of Preference by Similarity to Ideal Solution) method for finding shortest paths in a graph with multiple attributes.

## Features

- **Create Graph**: Allows users to create a graph with nodes and edges, specifying attributes for each node.
- **Apply TOPSIS**: Normalizes attribute values using the TOPSIS method to make them comparable.
- **Find Shortest Paths**: Utilizes Dijkstra's algorithm to find the shortest paths from a given source node, considering multiple attributes.

## TOPSIS (Technique for Order of Preference by Similarity to Ideal Solution)

TOPSIS is a multi-criteria decision-making method that determines the best alternative by considering both the similarity to the positive ideal solution and the distance from the negative ideal solution. 

![TOPSIS Example](topsis_example.png)

<!-- Insert image of TOPSIS example here -->

## Usage

1. **Compile**: Compile the program using a C compiler (e.g., gcc).
    ```bash
    gcc -o multi_attribute_dijkstra main.c -lm
    ```

2. **Run**: Execute the compiled program.
    ```bash
    ./multi_attribute_dijkstra
    ```

3. **Menu**: Follow the on-screen menu to create a graph, display its structure, and find shortest paths.

## Dependencies

- Standard C libraries: `stdio.h`, `stdlib.h`, `stdbool.h`, `math.h`

## Contributing

Contributions are welcome! If you have any suggestions or improvements, feel free to submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
