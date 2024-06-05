# File-Search

Custom implementation of the ls command in C

<img src="https://github.com/KimPiks/File-Search/blob/main/preview-image.png">

## Usage

```bash
./File_Search <path> <options>
```

## Options
* -l : Show full file information
* -R : Recursive search
* -a : Show hidden files
* -t : Sort by time
* -s : Sort by size

## Example
```bash
./File_Search /home -la
```

## Running
1. Clone the repository
```bash
git clone https://github.com/KimPiks/File-Search.git
```

2. Go to the project directory
```bash
cd File-Search
```

3. Build the project
```bash
cmake .
make
```

4. Run the program
```bash
./File_Search <path> <options>
```