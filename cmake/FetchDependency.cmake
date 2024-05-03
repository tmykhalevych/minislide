macro(FetchDependency NAME GIT_REPO GIT_TAG)
    string(TOUPPER ${NAME} NAME_UPPER)
    set(DEP_PATH "${NAME_UPPER}_PATH")
    
    # Check if the path is already set
    if (NOT ${DEP_PATH})
        message(STATUS "${NAME_UPPER}_PATH not specified, fetching ${NAME} from ${GIT_REPO}")
        
        include(FetchContent)
        FetchContent_Declare(${NAME}
            GIT_REPOSITORY ${GIT_REPO}
            GIT_TAG ${GIT_TAG}
            GIT_PROGRESS TRUE)
        
        # Check if the content has already been populated
        FetchContent_GetProperties(${NAME})
        if (NOT ${NAME}_POPULATED)
            set(FETCHCONTENT_QUIET FALSE)
            FetchContent_Populate(${NAME})
        endif()
        
        # Set the DEP_PATH variable
        string(TOLOWER ${NAME} NAME_LOWER)
        set(${DEP_PATH} ${${NAME_LOWER}_SOURCE_DIR} CACHE INTERNAL "${NAME} source directory")
        message(STATUS "${NAME_UPPER}_PATH set to '${DEP_PATH}'")
    endif()
endmacro()
