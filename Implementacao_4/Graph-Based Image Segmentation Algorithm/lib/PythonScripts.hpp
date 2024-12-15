#ifndef GENERATEPGM_HPP // Check if STRUCTURES_HPP is not defined
#define GENERATEPGM_HPP // Define STRUCTURES_HPP


#include <iostream>
#include <cstdlib> 
#include <string>

/** 
 * Simple function to generate pmg from an image file
 * 
 * A python script is run to create a new file pmg file on the project directory
 * 
 * The python script is expected to always be on the directory, and will return an error if not there
 * when the code is executed, and the string will also be controlled through the python script
 * 
 * @error When the Python script was not found, image does not or the Python script runs into some error
 */


bool isValidFilename(const std::string& filename) {
    // Check if the filename contains any invalid characters for the terminal
    for (char c : filename) {
        if (!std::isalnum(c) && c != '.' && c != '/' && c != '-' && c != '_') {
            return false;  // Invalid character found
        }
    }
    return true;
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();  // Check if the file exists and is readable
}

int executePythonScript(const std::string& pyFile) {
    // Check if the file name is valid
    if (!isValidFilename(pyFile)) {
        std::cerr << "Erro: Nome de arquivo inválido." << std::endl;
        return 1;  
    }

    // Check if the file exists
    if (!fileExists(pyFile)) {
        std::cerr << "Erro: Arquivo " << pyFile << " não encontrado." << std::endl;
        return 1; 
    }

    // Determine the appropriate command to run based on system
    const std::string commandPython = "python";
    const std::string commandDebian = "python3";

    const std::string command = "python " + pyFile;
    const std::string commandDebianFull = "python3 " + pyFile;

    // Try to run the command using python first
    int result = system(command.c_str());

    // If the first attempt fails, try the python3 command
    if (result != 0) {
        std::cerr << "Erro ao executar o script com 'python'. Tentando com 'python3'..." << std::endl;
        result = system(commandDebianFull.c_str());
    }

    // Check if the execution was successful
    if (result == 0) {
        std::cout << "Script Python executado com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro ao executar o script Python!" << std::endl;
    }

    return result;
}


const string pngToPgm3Script = "png_to_pgm.py";

int generatePgmFromFilename() {
    return executePythonScript(pngToPgm3Script);
}



const string pgm3ToPngScript = "pgm_to_png.py";

int pgmToFile() {
    return executePythonScript(pgm3ToPngScript);
}


#endif