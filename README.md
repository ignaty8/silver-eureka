# Sample C++ Project

### <Repository created as part of a coding exercise>

This repository contains a code sample able to process simple tasks in the following JSON format, printing the result to `stdout`:

```json
{
  "tasks": [
    {
      "type": "ADD",
      "num1": 12,
      "num2": 9
    },
    {
      "type": "SUB",
      "num1": 30,
      "num2": 8
    },
    {
      "type": "MUL",
      "num1": 3,
      "num2": 6
    },
    {
      "type": "DIV",
      "num1": 15,
      "num2": 3
    }
  ]
}
```

## Dependencies
Please insure you have a recent version of CMake (3.10 or higher) and a C++ compiler appropriate for your platform (C++14 or higher).
The source code relies on [Niels Lohmann's excellent JSON library](https://github.com/nlohmann/json). If you don't want to isntall it, you can just drop the source `json.hpp` file next to `main.cpp` and modify the relevant `#include` line accordingly.

## Installation

Run the compile.sh script (untested, but should work on Windows too).

```bash
./compile.sh
```

## Usage

You will find the compiled binary in the `build` directory.
Just pass it paths to input JSON files.
Every file's results will be printed on a new line to `stdout`.

```bash
./Main input1.json input2.json ...
```

## License
[MIT](https://choosealicense.com/licenses/mit/)