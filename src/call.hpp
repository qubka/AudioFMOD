#pragma once

template<typename Error, typename Function, typename... Params>
auto callImpl(const char* filename,
                uint32_t line,
                Error error,
                Function function,
                Params... params)
-> typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> {
    auto ret = function(std::forward<Params>(params)...);
#ifndef NDEBUG
    error(filename, line);
#endif
    return ret;
}

template<typename Error, typename Function, typename... Params>
auto callImpl(const char* filename,
                const uint32_t line,
                Error error,
                Function function,
                Params... params)
-> typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
    function(std::forward<Params>(params)...);
#ifndef NDEBUG
    return error(filename, line);
#else
    return true;
#endif
}
