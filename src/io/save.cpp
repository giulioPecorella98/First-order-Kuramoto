#include "save.h"

void saveSolution(const std::vector<Grid>& solution, int thetaPoints, size_t tPoints) {
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
        filename = "./saved_data/" + filename + ".bin";
        FILE* file = fopen(filename.c_str(), "wb");
        if (!file) {
            std::cerr << "Error opening file for writing." << std::endl;
            return;
        }
        fwrite(&thetaPoints, sizeof(int), 1, file);
	fwrite(&tPoints, sizeof(size_t), 1, file);
        for (const auto& f : solution) {
            fwrite(f.data(), sizeof(double), f.size(), file);
        }
        fclose(file);
        std::cout << "Result saved successfully in " << filename << "." << std::endl;
    }
}
