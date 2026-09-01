#pragma once

#define HEXCOL_TEAL     { 0.f, 0.5f, 0.5f, 1.f }
#define HEXCOL_TEAL_GREEN   { 0.420f, 0.627f, 0.545f, 1.0f }
#define HEXCOL_WHITE    { 1.f, 1.f, 1.f, 0.5f }

#define HEXCOL_RED          { 0.9098f, 0.4196f, 0.3608f, 1.0f }
#define HEXCOL_BLUE         { 0.3686f, 0.7059f, 0.8471f, 1.0f }
#define HEXCOL_DARK_BLUE    { 0.263f, 0.600f, 0.741f, 1.0f }
#define HEXCOL_BIEGE        { 0.953f, 0.922f, 0.875f, 0.9f }
#define HEXCOL_GREEN        { 0.3451f, 0.7922f, 0.6392f, 1.0f }
#define HEXCOL_PURPLE       { 0.7176f, 0.4157f, 0.8157f, 1.0f }
#define HEXCOL_YELLOW       { 0.8863f, 0.6784f, 0.3333f, 1.0f }
#define HEXCOL_TEAL_MATTE   { 0.47f, 0.62f, 0.63f, 1.f }


//---- for new glossy themes ----------
//#define HEXCOL_PINK   { 1.00f, 0.16f, 0.55f, 1.00f }
//#define HEXCOL_BLUE   { 0.05f, 0.72f, 0.92f, 1.00f }
//#define HEXCOL_ORANGE { 1.00f, 0.32f, 0.03f, 1.00f }
//
//#define HEXCOL_GREEN  { 0.08f, 0.72f, 0.08f, 1.00f }
//#define HEXCOL_PURPLE { 0.52f, 0.08f, 0.92f, 1.00f }
//#define HEXCOL_RED    { 0.94f, 0.03f, 0.12f, 1.00f }
//
//#define HEXCOL_YELLOW { 1.00f, 0.67f, 0.02f, 1.00f }
//#define HEXCOL_GRAY   { 0.60f, 0.63f, 0.68f, 1.00f }
//#define HEXCOL_BROWN  { 0.58f, 0.28f, 0.10f, 1.00f }


#define GET(_varType, _name, _var)                      \
    inline constexpr const _varType& get##_name() const {  \
        return _var;                                       \
    }

#define GETP(_varType, _name, _var)                      \
    inline constexpr _varType* get##_name() const {  \
        return _var;                                       \
    }

#define GETPREF(_varType, _name, _var)                      \
    inline constexpr _varType& get##_name() const {  \
        return *_var;                                       \
    }

#define SET(_varType, _name, _var)                         \
    inline constexpr void set##_name(const _varType& _arg) {  \
        _var = _arg;                                          \
    }

#define SETP(_varType, _name, _var)                         \
    inline constexpr void set##_name(_varType* _arg) {  \
        _var = _arg;                                          \
    }

#define GETB(_name, _var)              \
    inline constexpr const bool is##_name() const {     \
        return _var;                          \
    }