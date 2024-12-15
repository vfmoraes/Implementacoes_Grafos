# Image Segmentation Project

## Description

This project implements image segmentation using Python and C++. The Python script processes images, converts them into grayscale, and generates a PGM (Portable GrayMap) format file. The C++ program handles the execution of the Python script and provides a framework for running the image processing workflow.


## How to Run the Code

1. **Prerequisites**

    C++ Compiler: You need to have a C++ compiler installed (e.g., g++).

    Python: Ensure Python is installed on your system and accessible from the terminal. The script is compatible with both python and python3 commands.

    You can verify if Python is installed by running:

    ```bash
    python --version
    ```

    or

    ```bash
    python3 --version
    ```
    
    If Python is not installed, you can download and install it from [python.org](https://www.python.org).


2. **Instructions**:
   First, compile the `code.cpp` file by running the following command in your terminal:
   ```bash
   g++ code.cpp -o code
   ```

    After compiling the C++ code, you can run the executable:
    ./code

    This will execute the C++ code, which allows you to select one of images on the folder. The algorithm is run and the result is output on images folder:

    - The image is converted to gray scale and the result is displayed on `result_gray.png`
    - The pgm for the grayscale image is produced to be used internally, but its still acessible on `original.pgm`
    - An image marking different regions with different colors is displayed on `converted.png`
    - The result can also be viewed as pgm on `converted.pgm`, where each numeric value represents a different group

## Test Additional Images  

You can also test the program result on additional images that you include on the folder images.

Different k values for the algorithm are recommended for different image sizes. Per default the k values are as follow:

- sum of height and width <= 260: k = 150 
- sum of height and width <= 600: k = 300 
- else: k = 500

However for more robust results, different k values can be used by changing the threshold to a specific value on the `code.cpp` file

    