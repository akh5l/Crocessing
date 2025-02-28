#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "TextEditor.h"
#include "imgui-SFML.h"
#include "imgui.h"

#include <cstring>  // strings
#include <fstream>  // file streams
#include <future>   // threading
#include <iostream> // printing to console, debugging
#include <thread>   // threaded configure and build
#include <vector>   // arrays

TextEditor editor;

void setupEditor() {
  editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
  editor.SetPalette(TextEditor::GetLightPalette());

  editor.SetTabSize(4);
  editor.SetText(
      "\nvoid CSketch::setup() {\n\n}\nvoid CSketch::draw() {\n\n}\n");
}

// function to run a system command asynchronously
void run_command_async(const std::string &cmd) { std::system(cmd.c_str()); }

void writeFile(const std::vector<char> &code, std::string filePath) {
  std::cout << "Saving to " << filePath << std::endl;
  std::ofstream outFile(filePath);
  outFile.write("#include \"CSketch.hpp\"\n", 23);
  outFile.write("\n// Written in Crocessing.\n// github.com/akh5l/Crocessing\n\n", 59);
  outFile.write(code.data(), std::strlen(code.data()));
  outFile.close();
}

void execute_code(const std::vector<char> &code) {
  writeFile(code, "../app/testsketch.cpp");

  // run the compilation and execution asynchronously with a future
  std::vector<std::future<void>> futures;

  futures.push_back(std::async(std::launch::async, run_command_async,
                               "cmake -S ../app -B ../app/build"));
  futures.push_back(std::async(std::launch::async, run_command_async,
                               "cmake --build ../app/build"));

  for (auto &f : futures) {
    f.get(); // blocks only after starting all commands
  }

#ifdef _WIN32
  std::system(
      "start /B ../app/build/sketch1"); // Windows, run in the background
#else
  std::system("../app/build/sketch1 &"); // Linux/macOS, run in the background
#endif

  // Cleanup: remove the temporary files
  std::remove("../app/testsketch.cpp");
  std::remove("../app/build/sketch1");
}

int main() {
  sf::RenderWindow window(sf::VideoMode({720, 720}), "not an IDE",
                          sf::Style::Titlebar | sf::Style::Close |
                              sf::Style::Resize);
  window.setFramerateLimit(60);

  if (!ImGui::SFML::Init(window)) {
    std::cerr << "Failed to initialize ImGui-SFML" << std::endl;
    return -1;
  }
  ImGuiIO &io = ImGui::GetIO();
  io.Fonts->Clear(); // Clear existing fonts

  // Load the font from file (make sure the path is correct)
  io.Fonts->AddFontFromFileTTF("../font/Inter.ttf", 18.0f);

  // Reload font texture
  if (!ImGui::SFML::UpdateFontTexture()) {
    std::cerr << "Failed to update font texture" << std::endl;
    return -1;
  }
  ImGuiStyle &style = ImGui::GetStyle();
  style.FrameRounding = 15.0f;

  ImGui::GetStyle().Colors[ImGuiCol_Button] =
      ImVec4(0.547f, 0.516f, 0.418f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] =
      ImVec4(0.647f, 0.616f, 0.518f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] =
      ImVec4(0.447f, 0.416f, 0.318f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_FrameBg] =
      ImVec4(0.547f, 0.516f, 0.418f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_WindowBg] =
      ImVec4(0.707f, 0.677f, 0.581f, 1.0f);
  ImGui::GetStyle().Colors[ImGuiCol_Text] =
      ImVec4(0.971f, 0.963f, 0.951f, 1.0f);
  setupEditor();

  // code text area contents (mutable character array)
  std::vector<char> code(4096, 0); // Buffer with size

  // Main loop
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      ImGui::SFML::ProcessEvent(window, *event);
    }
    // start the ImGui frame
    ImGui::SFML::Update(window, sf::seconds(1.f / 60.f));

    // window.clear(sf::Color::White);
    // ImGui::ShowDemoWindow();       // to enable demo window
    // ImGui::SFML::Render(window);
    // window.display();
    // continue;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window.getSize().x),
                                    static_cast<float>(window.getSize().y)));

    ImGui::Begin("Processing-style IDE", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10)); // padding
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                        ImVec2(10, 10)); // item spacing

    // editor
    ImGui::Columns(1, "IDE", false);
    editor.Render("Code Editor",
                  ImVec2(window.getSize().x - 20,
                         window.getSize().y - 100), // space for buttons
                  false);

    std::string text = editor.GetText();
    code.assign(text.begin(), text.end());
    code.push_back('\0'); // null-terminate the vector

    // buttons
    ImGui::SetCursorPos(ImVec2(10, window.getSize().y - 80));

    if (ImGui::Button("Run", ImVec2(120, 40))) {
      std::cout << "Running code...\n";
      execute_code(code);
    }

    static char filePath[256] = "";
    ImGui::SameLine();
    if (ImGui::Button("Save", ImVec2(120, 40))) {
      if (strlen(filePath) > 0) {
        writeFile(code, filePath);
      }
    }

    ImGui::SameLine();
    ImGui::Text("File Path:");
    ImGui::SetNextItemWidth(200);
    ImGui::SameLine();
    ImGui::InputText("##FilePathInput", filePath, sizeof(filePath));

    // reset style changes for better UI consistency
    ImGui::PopStyleVar(2); // pop both style vars (Padding & Spacing)

    ImGui::End();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F5)) {
      execute_code(code);
    }

    // render everything
    window.clear(sf::Color::White);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
