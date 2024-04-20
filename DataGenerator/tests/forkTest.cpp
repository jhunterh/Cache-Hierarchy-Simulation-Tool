#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child1 = fork();

    if (child1 < 0) {
        // Fork failed
        std::cerr << "Fork failed for child 1... returning." << std::endl;
        return 1;
    } else if (child1 == 0) {
        std::cout << "Hello from child 1." << std::endl;
        return 0;
    } else {
        pid_t child2 = fork();

        if (child2 < 0) {
            std::cerr << "Fork failed for child 1... returning." << std::endl;
            return 1;
        } else if (child2 == 0) {
            std::cout << "Hello from child 2." << std::endl;
            return 0;
        } else {
            // wait for child processes to finish
            wait(NULL);
            wait(NULL);
            std::cout << "Hello from parent process." << std::endl;
            return 0;
        }
    }
}