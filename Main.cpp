#include <iostream>
#include "CustomVector.hpp"
#include <string>


int main() 
{

    std::cout << "Test 1: Integer Vector Operations\n";
    Vector<int> intVector;
    
    std::cout << "Empty vector: " << intVector << std::endl;
    std::cout << "Is empty: " << (intVector.empty() ? "Yes" : "No") << std::endl;
    std::cout << "Size: " << intVector.size() << std::endl;
    std::cout << "Capacity: " << intVector.capacity() << std::endl;

    intVector.push_back(10);
    intVector.push_back(20);
    intVector.push_back(30);
    std::cout << "After push_back: " << intVector << std::endl;
    std::cout << "Size: " << intVector.size() << std::endl;
    std::cout << "Capacity: " << intVector.capacity() << std::endl;

    std::cout << "\nTest 2: Copy Constructor and Assignment\n";
    Vector<int> copiedVector(intVector);
    std::cout << "Copied vector: " << copiedVector << std::endl;

    Vector<int> assignedVector;
    assignedVector = intVector;
    std::cout << "Assigned vector: " << assignedVector << std::endl;

    std::cout << "\nTest 3: Move Constructor and Assignment\n";
    Vector<int> movedVector(std::move(assignedVector));
    std::cout << "Moved vector: " << movedVector << std::endl;
    std::cout << "Moved vector size: " << movedVector.size() << std::endl;

    std::cout << "\nTest 4: Element Access and Iteration\n";
    std::cout << "First element: " << movedVector[0] << std::endl;
    
    std::cout << "Iteration with range-based for:\n";
    for (auto& elem : movedVector) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    std::cout << "\nTest 5: Erase Operation\n";
    movedVector.erase(1);
    std::cout << "After erasing second element: " << movedVector << std::endl;

    std::cout << "\nTest 6: String Vector\n";
    Vector<std::string> stringVector;
    stringVector.push_back("Hello");
    stringVector.push_back("World");
    stringVector.push_back("!");
    std::cout << "String vector: " << stringVector << std::endl;

    std::cout << "\nTest 7: Clear and Reset\n";
    stringVector.clear();
    std::cout << "After clear: " << stringVector << std::endl;
    std::cout << "Size after clear: " << stringVector.size() << std::endl;

    stringVector.push_back("Restored");
    std::cout << "After restoring: " << stringVector << std::endl;

    stringVector.reset();
    std::cout << "After reset: Size = " << stringVector.size() 
              << ", Capacity = " << stringVector.capacity() << std::endl;

    try {
        std::cout << "\nTest 8: Exception Handling\n";
        Vector<int> exceptionTest;
        std::cout << "Trying to access out-of-range index:\n";
        std::cout << exceptionTest[0] << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught out_of_range exception: " << e.what() << std::endl;
    }

    return 0;
}