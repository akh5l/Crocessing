#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "TextEditor.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

TextEditor editor;

void setupEditor() {
  editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
  editor.SetPalette(TextEditor::GetLightPalette());

  editor.SetTabSize(4);
  editor.SetText("\nvoid CSketch::setup() {\n\n}\nvoid CSketch::draw() {\n\n}\n");
}

void execute_code(const std::vector<char> &code) {
  // Create a temporary file to store the C++ code
  std::ofstream outFile("../app/testsketch.cpp");
  outFile.write("#include \"CSketch.hpp\"\n", 23);
  outFile.write(code.data(), std::strlen(code.data()));
  outFile.close();

  int configure_result = std::system("cmake -S ../app -B ../app/build");
  if (configure_result != 0) {
    std::cerr << "CMake configuration failed." << std::endl;
    return;
  }

  int build_result = std::system("cmake --build ../app/build");
  if (build_result != 0) {
    std::cerr << "Compilation failed." << std::endl;
    return;
  }

  int run_result = std::system("../app/build/sketch1");
  if (run_result != 0) {
    std::cerr << "Execution failed." << std::endl;
  }

  // Cleanup: remove the temporary files
  std::remove("../app/testsketch.cpp");
  std::remove("../app/build/sketch1");
}

int main() {
  // Create an SFML window that allows resizing and moving
  sf::RenderWindow window(sf::VideoMode({800, 600}), "not an IDE",
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
    ImGui::SetNextWindowSize(
        ImVec2(static_cast<float>(window.getSize().x),
               static_cast<float>(window.getSize().y))); // Fullscreen size

    // Begin the ImGui window with no title bar and borderless
    ImGui::Begin("Processing-style IDE", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    // Create a simple vertical layout
    ImGui::Columns(1, "IDE", false); // Single column layout
    editor.Render("Code Editor",
                  ImVec2(window.getSize().x - 20, window.getSize().y - 80),
                  false);

    std::string text = editor.GetText();
    code.assign(text.begin(), text.end());
    code.push_back('\0'); // null-terminate the vector

    // --- Fixed Position Buttons ---
    ImGui::SetCursorPos(ImVec2(
        10, window.getSize().y - 50)); // Position buttons at the bottom left
    if (ImGui::Button("Run", ImVec2(100, 40))) { // Wider buttons for better UX
      std::cout << "Running code...\n";
      execute_code(code);
    }

    // Add some space between buttons
    ImGui::SameLine(); // Place buttons on the same row

    if (ImGui::Button("Clear", ImVec2(100, 40))) {
      code.clear();
      code.push_back('\0'); // null-terminate, or inputtext will crash
    }

    ImGui::End(); // End the main window

    // Render everything
    window.clear(sf::Color::Black);
    ImGui::SFML::Render(window);
    window.display();
  }

  // Clean up and shutdown ImGui
  ImGui::SFML::Shutdown();
  return 0;
}
