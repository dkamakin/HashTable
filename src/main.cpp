#include "hashtableoutput.h"

// help for the user
void outputHelp(std::ostream &output) {
    output << "Choose one of the following actions: " << '\n';
    output << "1. Count the elements" << '\n';
    output << "2. Add the elements" << '\n';
    output << "3. Open a file" << '\n';
    output << "4. Close the file and read from std::cin" << '\n';
    output << "5. Delete an element" << '\n';
    output << "6. Resize the hashmap" << '\n';
    output << "7. Output the hashmap" << '\n';
    output << "8. Set advanced hash function" << '\n';
    output << "9. Set simple hash function" << '\n';
    output << "10. Exit" << '\n';
    output << "Your action: ";
}

// get an action from the user
int getAction(std::istream &input) {
    int action;
    outputHelp(std::cout);
    input >> action;
    input.ignore();
    return action;
}

// splits str on delimiter
std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> strings; // result
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) { // while can find delimiters
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start)); // get a substr and add to the result
    }

    return strings;
}

// get a string from the stream
void readString(std::istream &stream, std::string &string) {
    std::cout << "Input: ";
    getline(stream, string, '\n');
    std::cout << "Your string: " << string << '\n';
}

int main() {
    HashTable<std::string> table(10, std::make_shared<AdvancedState<std::string>>()); // hashtable
    HashTableOutput<std::string> tableOutput (table, &std::cout); // output information about main methods of the hashtable
    int action;
    int size;

    std::ifstream file; // file to read from
    std::string filePath; // path to the file
    std::string string; // input string
    std::vector<std::string> elements; // split input
    std::map<std::string, int> count; // count elements

    std::istream *input = &std::cin; // input stream

    while ((action = getAction(std::cin)) != 10) {

        switch (action) {
            case 1:
                readString(*input, string); // read input
                elements = split(string, ' '); // split input
                count = tableOutput.count(elements);

                for (const auto &elem : count) {
                    std::cout << "Elem (" << elem.first << ") contains " << elem.second << " times " << '\n';
                }

                break;
            case 2:
                readString(*input, string); // read string
                elements = split(string, ' '); // split input
                tableOutput.add(elements);
                break;
            case 3:
                if (file.is_open())
                    file.close();

                std::cout << "Path to the file: ";
                std::cin >> filePath; // read the file path
                file.open(filePath); // open file

                if (!file.is_open()) { // check if it opens
                    std::cout << "Couldn't open the file, please try again" << '\n';
                    continue;
                }

                input = &file; // change stream
                std::cout << "Now reading from the file (" << filePath << ")\n";
                break;
            case 4:
                if (file.is_open()) // close file if it was open
                    file.close();

                input = &std::cin; // change stream
                std::cout << "Now reading from the console" << '\n';
                break;
            case 5:
                readString(*input, string); // read input
                tableOutput.remove(string);
                std::cout << "\nAfter deleting: " << '\n';
                tableOutput.printTable(std::cout);
                break;
            case 6:
                std::cout << "Input new size: ";
                std::cin >> size;
                table.resize(size, &std::cout);
                tableOutput.printTable(std::cout);
                break;
            case 7:
                std::cout << "The table is: " << '\n';
                std::cout << tableOutput << '\n';
                break;
            case 8:
                table.setState(std::make_shared<AdvancedState<std::string>>());
                std::cout << "The hash function has been changed to advanced" << '\n';
                std::cout << "The hash is now calculated as follows (iterating over the element):"
                             " hash = 37 * hash (current) + element[i]" << '\n';
                break;
            case 9:
                table.setState(std::make_shared<SimpleState<std::string>>());
                std::cout << "The hash function has been changed to simple" << '\n';
                std::cout << "The hash is now calculated as follows (iterating over the element):"
                             " hash += element[i]" << '\n';
                break;
            case 10:
            default:
                std::cout << "Exiting the program" << '\n';
                return 0;
        }

        std::cout << '\n';
        if (file.is_open())
            std::cout << GREEN_COLOR << "Reading from the file (" << filePath << ')' << RESET << '\n';
        else
            std::cout << BLUE_COLOR << "Reading from the console" << RESET << '\n';
    }

    return 0;
}

