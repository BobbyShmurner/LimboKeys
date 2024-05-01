#include <atomic>
#include <thread>
#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "fonts/LatoBold.hpp"
#include "key_manager.hpp"
#include "state.hpp"
#include "colors.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"

#include "tracy/Tracy.hpp"

const unsigned int DEFAULT_WINDOW_WIDTH = (unsigned int)(120 * 1.5);
const unsigned int DEFAULT_WINDOW_HEIGHT = DEFAULT_WINDOW_WIDTH;
// const unsigned int DEFAULT_WINDOW_HEIGHT = (unsigned int)(84 * 1.5);

void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

void imgui_worker() {
	printf("Creating ImGui Thread...\n");

	if (!glfwInit()) {
		printf("Failed to initialize GLFW!\n");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(1, 1, "LIMBO", NULL, NULL);
	if (!window) {
		printf("Failed to Create GLFW Window!\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwHideWindow(window);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Failed to init GLEW: %s\n", glewGetErrorString(err));
		glfwTerminate();
		return;
	}

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.TabRounding = 8.f;
	style.FrameRounding = 8.f;
	style.GrabRounding = 8.f;
	style.WindowRounding = 8.f;
	style.PopupRounding = 8.f;
	style.TabBarBorderSize = 2.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	auto font13 = io.Fonts->AddFontFromMemoryCompressedTTF(LatoBoldFont_compressed_data, LatoBoldFont_compressed_size, 13.0f);
	auto font16 = io.Fonts->AddFontFromMemoryCompressedTTF(LatoBoldFont_compressed_data, LatoBoldFont_compressed_size, 16.0f);
	auto font24 = io.Fonts->AddFontFromMemoryCompressedTTF(LatoBoldFont_compressed_data, LatoBoldFont_compressed_size, 24.0f);
	auto font64 = io.Fonts->AddFontFromMemoryCompressedTTF(LatoBoldFont_compressed_data, LatoBoldFont_compressed_size, 64.0f);

	io.FontDefault = font16;
	io.Fonts->Build();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	bool showDemoWindow = false;

	while (!glfwWindowShouldClose(window) && State::instance()->running) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		if (showDemoWindow) {
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		{
        	ImGui::Begin("LIMBO", &State::instance()->running);

			ImGui::PushFont(font64);
            ImGui::Text("FOCUS");
			ImGui::PopFont();

            ImGui::Checkbox("Demo Window", &showDemoWindow);
            
			STATE_NOTIFY_IMGUI
			ImGui::Checkbox("Show Keys", &STATE_IMGUI_PARMS(State::instance()->showKeys));
			ImGui::Checkbox("Spin Keys", &State::instance()->spinKeys);
            STATE_NOTIFY_IMGUI
			ImGui::Checkbox("Decorate Key Windows", &STATE_IMGUI_PARMS(State::instance()->decorateKeyWindows));
			ImGui::SliderFloat("Reveal Amount", &State::instance()->revealAmount, 0.0f, 1.0f);
			ImGui::SliderFloat("Rotation", &State::instance()->rotation, 0.0f, 360.0f);

			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Circle Settings")) {
				ImGui::SliderFloat("Speed X", &State::instance()->speedX, -10.0f, 10.0f);
				ImGui::SliderFloat("Speed Y", &State::instance()->speedY, -10.0f, 10.0f);
				
				ImGui::SliderFloat("Amplitude X", &State::instance()->amplitudeX, -1.0f, 1.0f);
				ImGui::SliderFloat("Amplitude Y", &State::instance()->amplitudeY, -1.0f, 1.0f);
				
				ImGui::SliderFloat("Freq X", &State::instance()->freqX, -1.0f, 1.0f);
				ImGui::SliderFloat("Freq Y", &State::instance()->freqY, -1.0f, 1.0f);
			}

			ImGui::Spacing();

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

		ImGui::Render();

		int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
	}

	State::instance()->running = false;

	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwDestroyWindow(window);
}

int main() {
	printf("Loading GLFW...\n");

	if (!glfwInit()) {
		printf("Failed to initialize GLFW!\n");
		return -1;
	}

	glfwSetErrorCallback(glfw_error_callback);

	printf("Successfully initialized GLFW!\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(1, 1, "LIMBO", NULL, NULL);
	if (!window) {
		printf("Failed to Create GLFW Window!\n");
		glfwTerminate();
		return -1;
	}

	glfwHideWindow(window);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Failed to init GLEW: %s\n", glewGetErrorString(err));
		glfwTerminate();
		return -1;
	}

	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		fprintf(stderr, "[OpenGL]:%s %s\n", (type == GL_DEBUG_TYPE_ERROR ? " [ERROR]" : ""), message);
	}, NULL);

	printf("Loading ImGui...\n");

	std::thread imgui_thread(imgui_worker);

	printf("Creating Keys...\n");

	KeyManager* manager = new KeyManager("test");

	new Key(manager, Color::GREEN,	DEFAULT_WINDOW_WIDTH);
	new Key(manager, Color::YELLOW,	DEFAULT_WINDOW_WIDTH);
	new Key(manager, Color::BLUE,	DEFAULT_WINDOW_WIDTH);
	new Key(manager, Color::PURPLE,	DEFAULT_WINDOW_WIDTH);
	new Key(manager, Color::PINK,	DEFAULT_WINDOW_WIDTH);
	new Key(manager, Color::AQUA,	DEFAULT_WINDOW_WIDTH);
	new Key(manager, Color::LIME,	DEFAULT_WINDOW_WIDTH);
	new Key(manager, Color::RED,	DEFAULT_WINDOW_WIDTH);

	manager->initKeyWindows();
	RAISE_STATE_CHANGED(State::instance()->showKeys);

	printf("Created Keys!\n");

	while (!glfwWindowShouldClose(window) && State::instance()->running) {
		ZoneScoped;
		glfwPollEvents();

		if STATE_CHANGED(State::instance()->decorateKeyWindows) {
			for (auto key : manager->keys()) {
				key->setDecoration(State::instance()->decorateKeyWindows);
			}
		}

		if STATE_CHANGED(State::instance()->showKeys) {
			manager->setKeyVisibility(State::instance()->showKeys);
		}

		if (State::instance()->showKeys) {
			for (auto key : manager->keys()) {
				key->revealedAmount = State::instance()->revealAmount;
				key->rotation = State::instance()->rotation;

				if (State::instance()->spinKeys) {
					key->positonForCircle(glfwGetTime(), State::instance()->speedX, State::instance()->speedY, State::instance()->amplitudeX, State::instance()->amplitudeY, State::instance()->freqX, State::instance()->freqY);
				}
			}

			manager->render();
		}

		FrameMark;
	}

	State::instance()->running = false;

	delete manager;

	imgui_thread.join();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}