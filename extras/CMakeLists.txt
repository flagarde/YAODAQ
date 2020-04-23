find_package(Git REQUIRED)

find_package(Threads)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

add_subdirectory(CAEN)
