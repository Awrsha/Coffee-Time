#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <random>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

// Class representing a user
class User {
public:
    User(const std::string& username, const std::string& password)
        : username(username), password(password), loggedIn(false) {}

    const std::string& getUsername() const {
        return username;
    }

    bool isLoggedIn() const {
        return loggedIn;
    }

    // Method to attempt login
    bool login(const std::string& enteredPassword) {
        if (password == enteredPassword) {
            loggedIn = true;
            return true;
        }
        return false;
    }

    // Method to logout
    void logout() {
        loggedIn = false;
    }

private:
    std::string username;
    std::string password;
    bool loggedIn;
};

// Class representing a bank account
class Account {
public:
    Account(const std::string& accountHolder, double balance) : accountHolder(accountHolder), balance(balance) {}

    const std::string& getAccountHolder() const {
        return accountHolder;
    }

    double getBalance() const {
        return balance;
    }

    // Method to deposit money into the account
    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            return true;
        }
        return false;
    }

    // Method to withdraw money from the account
    bool withdraw(double amount) {
        if (balance >= amount && amount > 0) {
            balance -= amount;
            return true;
        }
        return false;
    }

    // Method to transfer money to another account
    bool transfer(Account& destination, double amount) {
        if (withdraw(amount)) {
            destination.deposit(amount);
            return true;
        }
        return false;
    }

private:
    std::string accountHolder;
    double balance;
};

// Class representing a database of accounts
class AccountDatabase {
public:
    // Method to add an account to the database
    void addAccount(const std::string& accountHolder, double balance) {
        accounts.emplace_back(accountHolder, balance);
    }

    // Method to check if an account exists
    bool accountExists(const std::string& accountHolder) const {
        for (const auto& account : accounts) {
            if (account.getAccountHolder() == accountHolder) {
                return true;
            }
        }
        return false;
    }

    // Method to get an account by its holder name
    Account* getAccount(const std::string& accountHolder) {
        for (auto& account : accounts) {
            if (account.getAccountHolder() == accountHolder) {
                return &account;
            }
        }
        return nullptr;
    }

private:
    std::vector<Account> accounts;
};

// Function to generate a random key
std::string generateRandomKey(int length = 16) {
    static const char charset[] = "0123456789"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

    std::string key;
    for (int i = 0; i < length; ++i) {
        key += charset[dis(gen)];
    }
    return key;
}

int main() {
    // Initialize the logger
    auto logger = spdlog::basic_logger_mt("banking_logger", "banking_log.txt");
    spdlog::set_default_logger(logger);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Banking System");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font file\n";
        spdlog::error("Failed to load font file.");
        return 1;
    }

    sf::Text title("Banking System", font, 40);
    title.setPosition(sf::Vector2f(250, 50));

    sf::Text instruction("Press L to log in, C to create an account, D to deposit money, W to withdraw money, T to transfer money, or Q to quit", font, 20);
    instruction.setPosition(sf::Vector2f(50, 150);

    AccountDatabase accounts;
    std::unordered_map<std::string, User> users;

    bool running = true;
    User* currentUser = nullptr;

    while (running) {
        window.clear();
        window.draw(title);
        window.draw(instruction);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) {
                    running = false;
                } else if (event.key.code == sf::Keyboard::L) {
                    if (currentUser) {
                        currentUser->logout();
                        currentUser = nullptr;
                    }

                    sf::Text usernameText("Enter username:", font, 20);
                    usernameText.setPosition(sf::Vector2f(50, 200));
                    sf::Text passwordText("Enter password:", font, 20);
                    passwordText.setPosition(sf::Vector2f(50, 250));
                    std::string inputUsername, inputPassword;
                    sf::Text inputUsernameText("", font, 20);
                    inputUsernameText.setPosition(sf::Vector2f(300, 200));
                    sf::Text inputPasswordText("", font, 20);
                    inputPasswordText.setPosition(sf::Vector2f(300, 250);

                    bool inputting = true;
                    while (inputting) {
                        window.clear();
                        window.draw(title);
                        window.draw(instruction);
                        window.draw(usernameText);
                        window.draw(passwordText);
                        window.draw(inputUsernameText);
                        window.draw(inputPasswordText);
                        window.display();

                        sf::Event inputEvent;
                        while (window.pollEvent(inputEvent)) {
                            if (inputEvent.type == sf::Event::TextEntered) {
                                if (inputEvent.text.unicode == '\b') {
                                    if (!inputUsername.empty()) {
                                        inputUsername.pop_back();
                                    }
                                    if (!inputPassword.empty()) {
                                        inputPassword.pop_back();
                                    }
                                } else if (inputEvent.text.unicode == '\r') {
                                    if (!inputUsername.empty() && !inputPassword.empty()) {
                                        std::string username = inputUsername;
                                        std::string password = inputPassword;

                                        if (users.find(username) != users.end() && users[username].login(password)) {
                                            currentUser = &users[username];
                                            inputting = false;
                                        } else {
                                            std::cerr << "Login failed: Invalid username or password." << std::endl;
                                            spdlog::error("Login failed: Invalid username or password.");
                                        }
                                    } else {
                                        std::cerr << "Login failed: Please enter both username and password." << std::endl;
                                        spdlog::error("Login failed: Please enter both username and password.");
                                    }
                                } else {
                                    if (inputUsername.size() < 50 && ((inputEvent.text.unicode >= 65 && inputEvent.text.unicode <= 90) || (inputEvent.text.unicode >= 97 && inputEvent.text.unicode <= 122))) {
                                        inputUsername += static_cast<char>(inputEvent.text.unicode);
                                    }
                                    if (inputPassword.size() < 50) {
                                        inputPassword += static_cast<char>(inputEvent.text.unicode);
                                    }
                                }
                                inputUsernameText.setString(inputUsername);
                                inputPasswordText.setString(std::string(inputPassword.size(), '*'));
                            }
                        }
                    }
                } else if (event.key.code == sf::Keyboard::C && currentUser) {
                    sf::Text accountHolderText("Enter account holder name:", font, 20);
                    accountHolderText.setPosition(sf::Vector2f(50, 200));
                    sf::Text balanceText("Enter starting balance:", font, 20);
                    balanceText.setPosition(sf::Vector2f(50, 250));
                    std::string inputAccountHolder, inputBalance;
                    sf::Text inputAccountHolderText("", font, 20);
                    inputAccountHolderText.setPosition(sf::Vector2f(300, 200));
                    sf::Text inputBalanceText("", font, 20);
                    inputBalanceText.setPosition(sf::Vector2f(300, 250);

                    bool inputting = true;
                    while (inputting) {
                        window.clear();
                        window.draw(title);
                        window.draw(instruction);
                        window.draw(accountHolderText);
                        window.draw(balanceText);
                        window.draw(inputAccountHolderText);
                        window.draw(inputBalanceText);
                        window.display();

                        sf::Event inputEvent;
                        while (window.pollEvent(inputEvent)) {
                            if (inputEvent.type == sf::Event::TextEntered) {
                                if (inputEvent.text.unicode == '\b') {
                                    if (!inputAccountHolder.empty()) {
                                        inputAccountHolder.pop_back();
                                    }
                                    if (!inputBalance.empty()) {
                                        inputBalance.pop_back();
                                    }
                                } else if (inputEvent.text.unicode == '\r') {
                                    if (!inputAccountHolder.empty() && !inputBalance.empty()) {
                                        std::string accountHolder = inputAccountHolder;
                                        double balance = std::stod(inputBalance);
                                        accounts.addAccount(accountHolder, balance);
                                        inputting = false;
                                    }
                                } else {
                                    if (inputAccountHolder.size() < 50 && ((inputEvent.text.unicode >= 65 && inputEvent.text.unicode <= 90) || (inputEvent.text.unicode >= 97 && inputEvent.text.unicode <= 122))) {
                                        inputAccountHolder += static_cast<char>(inputEvent.text.unicode);
                                    }
                                    if (inputBalance.size() < 10 && (inputEvent.text.unicode == '.' || (inputEvent.text.unicode >= 48 && inputEvent.text.unicode <= 57))) {
                                        inputBalance += static_cast<char>(inputEvent.text.unicode);
                                    }
                                }
                                inputAccountHolderText.setString(inputAccountHolder);
                                inputBalanceText.setString(inputBalance);
                            }
                        }
                    }
                } else if (event.key.code == sf::Keyboard::D && currentUser) {
                    sf::Text accountHolderText("Enter account holder name:", font, 20);
                    accountHolderText.setPosition(sf::Vector2f(50, 200));
                    sf::Text amountText("Enter amount to deposit:", font, 20);
                    amountText.setPosition(sf::Vector2f(50, 250));
                    std::string inputAccountHolder, inputAmount;
                    sf::Text inputAccountHolderText("", font, 20);
                    inputAccountHolderText.setPosition(sf::Vector2f(300, 200));
                    sf::Text inputAmountText("", font, 20);
                    inputAmountText.setPosition(sf::Vector2f(300, 250);

                    bool inputting = true;
                    while (inputting) {
                        window.clear();
                        window draw(title);
                        window.draw(instruction);
                        window.draw(accountHolderText);
                        window.draw(amountText);
                        window.draw(inputAccountHolderText);
                        window.draw(inputAmountText);
                        window.display();

                        sf::Event inputEvent;
                        while (window.pollEvent(inputEvent)) {
                            if (inputEvent.type == sf::Event::TextEntered) {
                                if (inputEvent.text.unicode == '\b') {
                                    if (!inputAccountHolder.empty()) {
                                        inputAccountHolder.pop_back();
                                    }
                                    if (!inputAmount.empty()) {
                                        inputAmount.pop_back();
                                    }
                                } else if (inputEvent.text.unicode == '\r') {
                                    if (!inputAccountHolder.empty() && !inputAmount.empty()) {
                                        std::string accountHolder = inputAccountHolder;
                                        double amount = std::stod(inputAmount);

                                        if (accounts.accountExists(accountHolder)) {
                                            Account* account = accounts.getAccount(accountHolder);
                                            if (account->deposit(amount)) {
                                                inputting = false;
                                            } else {
                                                std::cerr << "Deposit failed: Invalid amount." << std::endl;
                                                spdlog::error("Deposit failed: Invalid amount.");
                                            }
                                        } else {
                                            std::cerr << "Deposit failed: Account not found." << std::endl;
                                            spdlog::error("Deposit failed: Account not found.");
                                        }
                                    } else {
                                        std::cerr << "Deposit failed: Please enter both account holder and amount." << std::endl;
                                        spdlog::error("Deposit failed: Please enter both account holder and amount.");
                                    }
                                } else {
                                    if (inputAccountHolder.size() < 50 && ((inputEvent.text.unicode >= 65 && inputEvent.text.unicode <= 90) || (inputEvent.text.unicode >= 97 && inputEvent.text.unicode <= 122))) {
                                        inputAccountHolder += static_cast<char>(inputEvent.text.unicode);
                                    }
                                    if (inputAmount.size() < 10 && (inputEvent.text.unicode == '.' || (inputEvent.text.unicode >= 48 && inputEvent.text.unicode <= 57))) {
                                        inputAmount += static_cast<char>(inputEvent.text.unicode);
                                    }
                                }
                                inputAccountHolderText.setString(inputAccountHolder);
                                inputAmountText.setString(inputAmount);
                            }
                        }
                    }
                } else if (event.key.code == sf::Keyboard::W && currentUser) {
                    sf::Text accountHolderText("Enter account holder name:", font, 20);
                    accountHolderText.setPosition(sf::Vector2f(50, 200));
                    sf::Text amountText("Enter amount to withdraw:", font, 20);
                    amountText.setPosition(sf::Vector2f(50, 250));
                    std::string inputAccountHolder, inputAmount;
                    sf::Text inputAccountHolderText("", font, 20);
                    inputAccountHolderText.setPosition(sf::Vector2f(300, 200));
                    sf::Text inputAmountText("", font, 20);
                    inputAmountText.setPosition(sf::Vector2f(300, 250);

                    bool inputting = true;
                    while (inputting) {
                        window.clear();
                        window.draw(title);
                        window.draw(instruction);
                        window.draw(accountHolderText);
                        window.draw(amountText);
                        window.draw(inputAccountHolderText);
                        window.draw(inputAmountText);
                        window.display();

                        sf::Event inputEvent;
                        while (window.pollEvent(inputEvent)) {
                            if (inputEvent.type == sf::Event::TextEntered) {
                                if (inputEvent.text.unicode == '\b') {
                                    if (!inputAccountHolder.empty()) {
                                        inputAccountHolder pop_back();
                                    }
                                    if (!inputAmount.empty()) {
                                        inputAmount.pop_back();
                                    }
                                } else if (inputEvent.text.unicode == '\r') {
                                    if (!inputAccountHolder.empty() && !inputAmount.empty()) {
                                        std::string accountHolder = inputAccountHolder;
                                        double amount = std::stod(inputAmount);

                                        if (accounts.accountExists(accountHolder)) {
                                            Account* account = accounts.getAccount(accountHolder);
                                            if (account->withdraw(amount)) {
                                                inputting = false;
                                            } else {
                                                std::cerr << "Withdrawal failed: Insufficient balance or invalid amount." << std::endl;
                                                spdlog::error("Withdrawal failed: Insufficient balance or invalid amount.");
                                            }
                                        } else {
                                            std::cerr << "Withdrawal failed: Account not found." << std::endl;
                                            spdlog::error("Withdrawal failed: Account not found.");
                                        }
                                    } else {
                                        std::cerr << "Withdrawal failed: Please enter both account holder and amount." << std::endl;
                                        spdlog::error("Withdrawal failed: Please enter both account holder and amount.");
                                    }
                                } else {
                                    if (inputAccountHolder.size() < 50 && ((inputEvent.text.unicode >= 65 && inputEvent.text.unicode <= 90) || (inputEvent.text.unicode >= 97 && inputEvent.text.unicode <= 122))) {
                                        inputAccountHolder += static_cast<char>(inputEvent.text.unicode);
                                    }
                                    if (inputAmount.size() < 10 && (inputEvent.text.unicode == '.' || (inputEvent.text.unicode >= 48 && inputEvent.text.unicode <= 57))) {
                                        inputAmount += static_cast<char>(inputEvent.text.unicode);
                                    }
                                }
                                inputAccountHolderText.setString(inputAccountHolder);
                                inputAmountText.setString(inputAmount);
                            }
                        }
                    }
                } else if (event.key.code == sf::Keyboard::T && currentUser) {
                    sf::Text sourceAccountText("Enter source account holder name:", font, 20);
                    sourceAccountText.setPosition(sf::Vector2f(50, 200));
                    sf::Text destinationAccountText("Enter destination account holder name:", font, 20);
                    destinationAccountText.setPosition(sf::Vector2f(50, 250));
                    sf::Text amountText("Enter amount to transfer:", font, 20);
                    amountText.setPosition(sf::Vector2f(50, 300));
                    std::string inputSourceAccount, inputDestinationAccount, inputAmount;
                    sf::Text inputSourceAccountText("", font, 20);
                    inputSourceAccountText.setPosition(sf::Vector2f(400, 200));
                    sf::Text inputDestinationAccountText("", font, 20);
                    inputDestinationAccountText.setPosition(sf::Vector2f(400, 250));
                    sf::Text inputAmountText("", font, 20);
                    inputAmountText.setPosition(sf::Vector2f(300, 300);

                    bool inputting = true;
                    while (inputting) {
                        window.clear();
                        window.draw(title);
                        window.draw(instruction);
                        window.draw(sourceAccountText);
                        window.draw(destinationAccountText);
                        window.draw(amountText);
                        window.draw(inputSourceAccountText);
                        window.draw(inputDestinationAccountText);
                        window.draw(inputAmountText);
                        window.display();

                        sf::Event inputEvent;
                        while (window.pollEvent(inputEvent)) {
                            if (inputEvent.type == sf::Event::TextEntered) {
                                if (inputEvent.text.unicode == '\b') {
                                    if (!inputSourceAccount.empty()) {
                                        inputSourceAccount.pop_back();
                                    }
                                    if (!inputDestinationAccount.empty()) {
                                        inputDestinationAccount.pop_back();
                                    }
                                    if (!inputAmount.empty()) {
                                        inputAmount.pop_back();
                                    }
                                } else if (inputEvent.text.unicode == '\r') {
                                    if (!inputSourceAccount.empty() && !inputDestinationAccount.empty() && !inputAmount.empty()) {
                                        std::string sourceAccountHolder = inputSourceAccount;
                                        std::string destinationAccountHolder = inputDestinationAccount;
                                        double amount = std::stod(inputAmount);

                                        if (accounts.accountExists(sourceAccountHolder) && accounts.accountExists(destinationAccountHolder)) {
                                            Account* sourceAccount = accounts.getAccount(sourceAccountHolder);
                                            Account* destinationAccount = accounts.getAccount(destinationAccountHolder);

                                            if (sourceAccount->transfer(*destinationAccount, amount)) {
                                                inputting = false;
                                            } else {
                                                std::cerr << "Transfer failed: Insufficient balance or invalid amount." << std::endl;
                                                spdlog::error("Transfer failed: Insufficient balance or invalid amount.");
                                            }
                                        } else {
                                            std::cerr << "Transfer failed: One or both accounts not found." << std::endl;
                                            spdlog::error("Transfer failed: One or both accounts not found.");
                                        }
                                    } else {
                                        std::cerr << "Transfer failed: Please enter both account holders and amount." << std::endl;
                                        spdlog::error("Transfer failed: Please enter both account holders and amount.");
                                    }
                                } else {
                                    if (inputSourceAccount.size() < 50 && ((inputEvent.text.unicode >= 65 && inputEvent.text.unicode <= 90) || (inputEvent.text.unicode >= 97 && inputEvent.text.unicode <= 122))) {
                                        inputSourceAccount += static_cast<char>(inputEvent.text.unicode);
                                    }
                                    if (inputDestinationAccount.size() < 50 && ((inputEvent.text.unicode >= 65 and inputEvent.text.unicode <= 90) or (inputEvent.text.unicode >= 97 and inputEvent.text.unicode <= 122))) {
                                        inputDestinationAccount += static_cast<char>(inputEvent.text.unicode);
                                    }
                                    if (inputAmount.size() < 10 && (inputEvent.text.unicode == '.' || (inputEvent.text.unicode >= 48 and inputEvent.text.unicode <= 57))) {
                                        inputAmount += static_cast<char>(inputEvent.text.unicode);
                                    }
                                }
                                inputSourceAccountText.setString(inputSourceAccount);
                                inputDestinationAccountText.setString(inputDestinationAccount);
                                inputAmountText.setString(inputAmount);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
