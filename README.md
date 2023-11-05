# Simple C++ Application Plugin Framework
Simple C++ Application Plugin Framework ('The Framework' from here-on) is a minimal implementation example of a C++ application that supports a simple but robust plugin system (Hot-Plug no supported) with dependency resolution as well as a simple messaging system (event propagation).

The Framework was created to provide myself with a quick-start project for simple applications that requires this functionality.

### Features
* C++17
* CMake based build system
* Shared Common Library
* Generic Dependency Resolver
* Automated Plugin Registration
* Simple Extensible Messaging for events

### Contents
* **Executable**
  * Contains the core functionality of the application
  * Owns the Plugin Manager and Message Manager
  * Loads and Unloads plugins
* **Common**
  * Contains shared functionality to be used between plugins and  the application
  * Plugin Framework
  * Message Framework
  * Guid implementation
  * Dependency Resolver Implementation
* **Plugin 1**
  * Sample plugin, depends on Plugin 3
* **Plugin 2**
  * Sample plugin, depends on Plugin 1 and Plugin 3
* **Plugin 3**
  * Sample plugin, no dependencies

### Building
**Requirements**
* Visual Studio 2022 (preferred) / (Any C++17 compatible compiler)
* CMake 3.8+
* Windows 10 / Windows 11

**Steps**
1. Clone the repository
2. Execute Generate_VS2022.cmd
3. Open the solution\PluginFramework.sln
4. Build Debug or Release

### Contributions
Although this project was only developed to give myself a starting point for new applications that require this framework, I do welcome contributions that;
1. Simplify the implementations without sacrificing readability and/or functionality
2. Extend platform support without over-complicating or refactoring current implementations
3. Stabilize or other-wise fix errors/bugs in the machinery