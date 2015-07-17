#include <stdio.h>
#include <string.h>

// TODO: Make thread safe.
class BasicFile {
public:
    BasicFile() : file(nullptr)
    {
    }

    virtual ~BasicFile()
    {
        Close();
    }

    void OpenOrCreate(const char* fileName)
    {
        // "r" read: Open file for input operations. The file must exist.
        // "w" write: Create an empty file for output operations. If a file with
        //     the same name already exists, its contents are discarded and the
        //     file is treated as a new empty file.
        // "a" append: Open file for output at the end of a file. Output operations
        //     always write data at the end of the file, expanding it. Repositioning
        //     operations (fseek, fsetpos, rewind) are ignored. The file is created
        //     if it does not exist.
        // "r+" read/update: Open a file for update (both for input and output). The
        //     file must exist.
        // "w+" write/update: Create an empty file and open it for update (both for
        //     input and output). If a file with the same name already exists its
        //     contents are discarded and the file is treated as a new empty file.
        // "a+" append/update: Open a file for update (both for input and output)
        //     with all output operations writing data at the end of the file.
        //     Repositioning operations (fseek, fsetpos, rewind) affects the next
        //     input operations, but output operations move the position back to the
        //     end of file. The file is created if it does not exist.
        file = fopen(fileName, "a+");
        if (!file)
        {
            printf("Cannot open file.\n");
        }
    }

    void Close()
    {
        if (file)
        {
            fclose(file);
            file = nullptr;
        }
    }

    void Append(char* text)
    {
        fwrite(text, sizeof(text[0]), strlen(text), file);
    }

    char* ReadLine(char* buffer, int sizeOfBuffer)
    {
        char* result = fgets(buffer, sizeOfBuffer, file);
        if (!result && !feof(file))
        {
            printf("An error ocurred while reading the file.\n");
        }

        return result;
    }

private:
    FILE* file;
};

int main(int argc, char* argv[])
{
    BasicFile file;

    // Write example.
    file.OpenOrCreate("foo.txt");
    for (int i = 1; i <= 10; i++)
    {
        char buffer[255];
        sprintf(buffer, "Line %d\n", i);
        file.Append(buffer);
    }
    file.Close();

    // Read example.
    file.OpenOrCreate("foo.txt");
    char* result = nullptr;
    char buffer[255];
    result = file.ReadLine(buffer, sizeof(buffer));
    while (result)
    {
        printf(result);
        result = file.ReadLine(buffer, sizeof(buffer));
    }
    file.Close();

    return 0;
}
