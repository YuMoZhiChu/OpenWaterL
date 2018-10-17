cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project(GLAD)

file(GLOB SOURCE
		"./src/*.c"
	)

include_directories("./include")
	
add_library(glad ${SOURCE})