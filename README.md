# OptimizedSRCS

## Project Contributors

Created as a school project by:

- [@RigelKukoy](https://github.com/RigelKukoy)
- [@keyceerhaye](https://github.com/keyceerhaye)

## Contributing

Contributions are welcome! Here are some ways you can contribute:

1. Improve the algorithm's performance
2. Add support for different data types
3. Enhance error handling
4. Improve documentation
5. Add test cases

### Contributing Guidelines

1. Fork the repository
2. Create a new branch for your feature
3. Commit your changes
4. Push to your branch
5. Create a Pull Request

### Code Style

- Use consistent indentation (4 spaces)
- Add comments for complex logic
- Follow C++ best practices
- Include appropriate error handling

## Performance Considerations

- The algorithm's performance depends on:
  - Input data distribution
  - Number of processes used
  - Available system resources
  - Network speed (for distributed systems)

## Known Limitations

1. The number of elements should be divisible by the number of processes
2. Currently only supports integer values

## Acknowledgments

- List any references or inspirations for the algorithm
- Credit any contributors or related works

## Building and Running

### Building the Code

````bash
# Basic compilation
mpic++ -o MPISRCS MPISRCS.cpp -std=c++11

### Running the Program

The program can be run with different numbers of processes using the `mpirun` command:

```bash
# Basic syntax
mpirun -np <number_of_processes> ./MPISRCS

# Examples:
# Run with 4 processes
mpirun -np 4 ./MPISRCS

# Run with 8 processes
mpirun -np 8 ./MPISRCS

# Run on multiple nodes (if available)
mpirun -np 16 --hostfile hostfile ./MPISRCS
````

### Performance Tips

The optimal number of processes depends heavily on:

1. Your input data characteristics:
   - Data size
   - Data distribution
   - Range of values
2. Your system configuration:
   - Available CPU cores
   - Memory capacity

To find the optimal process count for your specific case:

1. Start with a small sample of your data
2. Run benchmarks with different process counts (2, 4, 8, 16...)
3. Monitor:
   - Total execution time
   - Memory usage
4. Choose the configuration that gives the best performance/resource trade-off

## Research Paper Reference

This implementation is based on the research paper:
"Parallel Square Root Counting Sort Algorithm Using MPI"
[Link to paper](https://drive.google.com/file/d/11ZOY1lcs8r6FsCpapZgly84vlF_MAJcc/view)

### Algorithm Visualization

<p align="center">
  <img src="/images/introduction.png" alt="SRCS Algorithm Overview"/>
  <br>
  <em>Figure 1: Paper introduction</em>
</p>

<p align="center">
  <img src="/images/performance.png" alt="Performance Results"/>
  <br>
  <em>Figure 2: Experimental results</em>
</p>

### Troubleshooting

If you encounter errors:

1. Check that MPI is properly installed:

   ```bash
   which mpirun
   mpirun --version
   ```

2. Verify process count is valid:

   - Must be at least 2
   - Must not exceed available resources
   - Should divide evenly into your data size

3. Common error solutions:

   ```bash
   # If permission denied
   chmod +x MPISRCS

   # If hostfile needed
   echo "localhost slots=4" > hostfile
   mpirun -np 4 --hostfile hostfile ./MPISRCS
   ```

## Project Contributors

Created as a school project by:

- [@RigelKukoy](https://github.com/RigelKukoy)
- [@keyceerhaye](https://github.com/keyceerhaye)
