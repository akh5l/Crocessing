#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "TextEditor.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

TextEditor editor;

void setupEditor() {
  editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
  editor.SetPalette(TextEditor::GetLightPalette());

  editor.SetTabSize(4);
  editor.SetText(
      "\nvoid CSketch::setup() {\n\n}\nvoid CSketch::draw() {\n\n}\n");
}

// Function to run a system command asynchronously
void run_command_async(const std::string &cmd) { std::system(cmd.c_str()); }

void writeFile(const std::vector<char> &code, std::string filePath) {
  std::cout << "Saving to " << filePath << std::endl;
  std::ofstream outFile(filePath);
  outFile.write("#include \"CSketch.hpp\"\n", 23);
  outFile.write(code.data(), std::strlen(code.data()));
  outFile.close();
}

void execute_code(const std::vector<char> &code) {
  // Create a temporary file to store the C++ code
  writeFile(code, "../app/testsketch.cpp");

  // Use a future to run the compilation and execution asynchronously
  std::vector<std::future<void>> futures;

  futures.push_back(std::async(std::launch::async, run_command_async,
                               "cmake -S ../app -B ../app/build"));
  futures.push_back(std::async(std::launch::async, run_command_async,
                               "cmake --build ../app/build"));

  // Wait for all commands to finish
  for (auto &f : futures) {
    f.get(); // Will block only after starting all commands
  }

// Now, run the executable in the background:
#ifdef _WIN32
  std::system(
      "start /B ../app/build/sketch1"); // On Windows, run in the background
                                        // without opening a new terminal window
#else
  std::system(
      "../app/build/sketch1 &"); // On Linux/macOS, run in the background
#endif

  // Cleanup: remove the temporary files
  std::remove("../app/testsketch.cpp");
  // std::remove("../app/build/sketch1");
}

int main() {
  // Create an SFML window that allows resizing and moving
  sf::RenderWindow window(sf::VideoMode({720, 720}), "not an IDE",
                          sf::Style::Titlebar | sf::Style::Close |
                              sf::Style::Resize);

  // Setup ImGui for SFML
  ImGui::SFML::Init(window);
  ImGuiIO &io = ImGui::GetIO();
  io.Fonts->Clear(); // Clear existing fonts

  // Load the font from file (make sure the path is correct)
  io.Fonts->AddFontFromFileTTF("../font/Inter.ttf", 18.0f);

  // Reload font texture
  ImGui::SFML::UpdateFontTexture();
  ImGui::GetStyle().Colors[ImGuiCol_Button] =
      ImVec4(0.647f, 0.616f, 0.518f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] =
      ImVec4(0.7f, 0.65f, 0.57f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] =
      ImVec4(0.547f, 0.516f, 0.418f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_FrameBg] =
      ImVec4(0.647f, 0.616f, 0.518f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_WindowBg] =
      ImVec4(0.757f, 0.727f, 0.631f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_Text] =
      ImVec4(0.971f, 0.963f, 0.951f, 1.0f);
  setupEditor();
  window.setFramerateLimit(60);

  // Code text area contents (mutable character array)
  std::vector<char> code(4096, 0); // Buffer with size

  // Main loop
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      ImGui::SFML::ProcessEvent(window, *event);
    }
    // Start the ImGui frame
    ImGui::SFML::Update(window, sf::seconds(1.f / 60.f));

    // Set the next window position and size to fullscreen
    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Position at top-left
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window.getSize().x),
                                    static_cast<float>(window.getSize().y)));

    // Begin the ImGui window with no title bar and borderless
    ImGui::Begin("Processing-style IDE", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove);

    // Create a simple vertical layout with more padding and space
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
                        ImVec2(10, 10)); // Add padding
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                        ImVec2(10, 10)); // Add item spacing

    // Editor (take up most of the space)
    ImGui::Columns(1, "IDE", false); // Single column layout
    editor.Render("Code Editor",
                  ImVec2(window.getSize().x - 20,
                         window.getSize().y - 100), // Leave space for buttons
                  false);

    // Retrieve the code from the editor and update the code vector
    std::string text = editor.GetText();
    code.assign(text.begin(), text.end());
    code.push_back('\0'); // null-terminate the vector

    // --- Fixed Position Buttons ---
    ImGui::SetCursorPos(ImVec2(
        10, window.getSize().y - 80)); // Position buttons at the bottom left

    // Run Button
    if (ImGui::Button("Run", ImVec2(120, 40))) { // Wider buttons for better UX
      std::cout << "Running code...\n";
      execute_code(code);
    }

    ImGui::SameLine();
    // File Path Input (Textbox)
    ImGui::Text("File Path:");    // Display the label
    ImGui::SetNextItemWidth(200); // Set the width of the input field
    ImGui::SameLine();
    static char filePath[256] = "";
    ImGui::InputText("##FilePathInput", filePath, sizeof(filePath));

    // Save Button
    ImGui::SameLine();
    if (ImGui::Button("Save", ImVec2(120, 40))) {
      // Check if the file path is valid
      if (strlen(filePath) > 0) {
        writeFile(code, filePath);
      }
    }

    // Reset style changes for better UI consistency
    ImGui::PopStyleVar(2); // Pop both style vars (Padding & Spacing)

    ImGui::End(); // End the main window

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F5)) {
      execute_code(code); // Trigger code execution logic
    }

    // Render everything
    window.clear(sf::Color::Black);
    ImGui::SFML::Render(window);
    window.display();
  }

  // Clean up and shutdown ImGui
  ImGui::SFML::Shutdown();
  return 0;
}
