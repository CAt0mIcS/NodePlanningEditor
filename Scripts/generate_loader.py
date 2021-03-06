import sys
import os


loader_file = sys.argv[1] + "/Ray/Core/RRendererLoader" if len(
    sys.argv) > 1 else "../Ray/Core/RRendererLoader"
header_file_directories = [
    sys.argv[1] + "/RayRenderer/Core", sys.argv[1] + "/RayRenderer/Synchronization", sys.argv[1] + "/RayRenderer/Pipeline"] if len(sys.argv) > 1 else ["../RayRenderer/Core", "../RayRenderer/Synchronization", "../RayRenderer/Pipeline"]
loader_template_cpp = r"""

#include "Rpch.h"
#include "RRendererLoader.h"

#include "Utils/RException.h"

#ifdef _WIN32
    #include <Windows.h>
    #ifdef CreateSemaphore
        #undef CreateSemaphore
    #endif
#else
    #include <dlfcn.h>
#endif


namespace At0::Ray
{
	namespace RendererAPI
	{
		Type API;
{function_definitions}
	}  // namespace RendererAPI


    template<typename... Args>
	void* LoadFunction(Args&&... args)
	{
#ifdef _WIN32
		return GetProcAddress(std::forward<Args>(args)...);
#else
		return dlsym(std::forward<Args>(args)...);
#endif
	}

	void LoadRenderer(RendererAPI::Type type)
	{
#ifdef _WIN32
		HMODULE lib = nullptr;
        switch (type)
		{
		case RendererAPI::OpenGL: lib = LoadLibraryA("RayRendererOpenGL.dll"); break;
		case RendererAPI::Vulkan: lib = LoadLibraryA("RayRendererVulkan.dll"); break;
		default: ThrowRuntime("Invalid renderer type {0}", (uint32_t)type);
		}
#else
        void* lib = nullptr;
        switch (type)
		{
		case RendererAPI::OpenGL: lib = dlopen("libRayRendererOpenGL.so", RTLD_LAZY); break;
		case RendererAPI::Vulkan: lib = dlopen("libRayRendererVulkan.so", RTLD_LAZY); break;
		default: ThrowRuntime("Invalid renderer type {0}", (uint32_t)type);
		}
#endif


		if (!lib)
		{
			std::string rendererStr;
			switch (type)
			{
			case RendererAPI::OpenGL: rendererStr = "OpenGL"; break;
			case RendererAPI::Vulkan: rendererStr = "Vulkan"; break;
			}

			ThrowRuntime("[Loader] Failed to load {0} renderer", rendererStr);
		}

{function_assignments}

		RendererAPI::API = type;
	}
}  // namespace At0::Ray

"""

loader_template_h = r"""
#pragma once

#include "Ray/RBase.h"
{includes}


namespace At0::Ray
{
	namespace RendererAPI
	{
		enum Type
		{
			OpenGL,
			Vulkan
		};


		extern Type API;
{function_declarations}
	}  // namespace RendererAPI

	RAY_EXPORT void LoadRenderer(RendererAPI::Type type);
}  // namespace At0::Ray

"""


sources = list(list())
header_files = list()


class Function:
    def __init__(self, function: str):
        self.function = function

        function = function.replace("RR_API ", "")
        self.return_type = function.split(" ")[0]
        self.raw_name = function.split(" ")[1].split("(")[0]
        if "Rr" == self.raw_name[:2]:
            self.name = self.raw_name[2:]
        else:
            self.name = self.raw_name

        self.args = function.split("(")[1].replace(
            ");", "").replace("\n", "").replace("\t", "")
        self.function_type_with_name = f"{self.return_type} (*{self.name})({self.args})"
        self.function_type = f"{self.return_type} (*)({self.args})"


def load_sources():
    raw_sources = list(list())
    raw_header_files = list()
    for header_file_dir in header_file_directories:
        raw_header_files += ([os.path.join(dp, f) for dp, dn, filenames in os.walk(
            header_file_dir) for f in filenames if os.path.splitext(f)[1] == '.h'])

    for header_file in raw_header_files:
        with open(header_file, "r") as reader:
            raw_sources.append(reader.readlines())

    for i in range(0, len(raw_sources)):
        sources.append(list())
        for j in range(0, len(raw_sources[i])):
            sources[i].append("")
            lineStr = raw_sources[i][j]
            if "RR_API " in lineStr:
                line = 0
                while ";" not in raw_sources[i][j + line]:
                    sources[i][j] += (raw_sources[i][j + line])
                    line += 1
                sources[i][j] += (raw_sources[i][j + line])
                if raw_header_files[i] not in header_files:
                    header_files.append(raw_header_files[i])

    # Remove empty lists
    for source in sources:
        while "" in source:
            source.remove("")

    i = 0
    for _ in range(0, len(sources)):
        if i < 0:
            i = 0
        if len(sources[i]) == 0:
            sources.remove(sources[i])
            i -= 1
        else:
            i += 1


def build_function_declarations():
    declarations = list()

    includes = ""
    for header_file in header_files:
        includes += f"#include <{header_file}>\n"

    while "\\" in includes:
        includes = includes.replace("\\", "/")

    for source in sources:
        for raw_function in source:
            function = Function(raw_function)
            declarations.append(
                f"\t\textern {function.function_type_with_name};")

    template = ""
    for declaration in declarations:
        template += declaration + "\n"
    template = loader_template_h.replace(
        "{function_declarations}", template)

    template = template.replace("{includes}", includes)

    with open(loader_file + ".h", "w") as writer:
        writer.write(template)


def build_function_definitions_and_assignments():
    definitions = list()
    assignments = list()

    for source in sources:
        for raw_function in source:
            function = Function(raw_function)
            definitions.append(
                f"\t\t{function.function_type_with_name} = nullptr;")
            assignments.append(
                f"\t\tRendererAPI::{function.name} = ({function.function_type})LoadFunction(lib, \"{function.raw_name}\");")

    template_defs = ""
    for definition in definitions:
        template_defs += definition + "\n"
    template_defs = loader_template_cpp.replace(
        "{function_definitions}", template_defs)

    template_assigns = ""
    for assignment in assignments:
        template_assigns += assignment + "\n"
    template_defs = template_defs.replace(
        "{function_assignments}", template_assigns)

    with open(loader_file + ".cpp", "w") as writer:
        writer.write(template_defs)


if __name__ == "__main__":
    load_sources()
    build_function_declarations()
    build_function_definitions_and_assignments()
