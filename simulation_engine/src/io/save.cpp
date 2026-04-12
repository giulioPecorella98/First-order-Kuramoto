#include "save.h"

void saveSolution(const std::vector<Grid>& solution, int thetaPoints, size_t tPoints, double T) {
    std::cout << "Do you wish to save the result? (y or n)" << std::endl;
    char saveChoice;
    std::cin >> saveChoice;
    while (((saveChoice != 'y') && (saveChoice != 'Y')) && ((saveChoice != 'n') && (saveChoice != 'n')) || (std::cin.fail())) {
        std::cout << "Invalid choice. Please enter y or n: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> saveChoice;
    }
    if ((saveChoice == 'y') || (saveChoice == 'Y')) {
        std::cout << "Enter the name of the file (without extension) where you want to save the result: ";
        std::string filename;
        std::cin >> filename;
        std::filesystem::path fullpath = std::filesystem::path(PROJECT_ROOT) / "saved_data" / (filename + ".bin");
        FILE* file = fopen(fullpath.string().c_str(), "wb");
        if (!file) {
            std::cerr << "Error: file not saved. "<< std::endl;
            return;
        }
        fwrite(&thetaPoints, sizeof(int), 1, file);
	    fwrite(&tPoints, sizeof(size_t), 1, file);
        fwrite(&T, sizeof(double), 1, file);        
        for (const auto& f : solution) {
            fwrite(f.data(), sizeof(double), f.size(), file);
        }
        fclose(file);
        std::cout << "Result saved successfully in " << fullpath.generic_string() << "." << std::endl;
    }
}
