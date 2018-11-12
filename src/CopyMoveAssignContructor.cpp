#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

int idx = 0;

class MemoryBlock {
public:
    int id = idx++;

    // Simple constructor that initializes the resource.
    explicit MemoryBlock(size_t length) :
            _length(length), _data(new int[length]) {
        std::cout << "Id = [" << id << "]. In MemoryBlock(size_t). length = "
                << _length << "." << std::endl;
    }

    // Destructor.
    ~MemoryBlock() {
        std::cout << "Id = [" << id << "].  ~MemoryBlock(). length = "
                << _length << ".";

        if (_data != nullptr) {
            std::cout << " Deleting resource.";
            // Delete the resource.
            delete[] _data;
        }

        std::cout << std::endl;
    }

    // Copy constructor.
    MemoryBlock(const MemoryBlock& other) :
            _length(other._length), _data(new int[other._length]) {
        std::cout << "Id = [" << id
                << "]. In MemoryBlock(const MemoryBlock&). length = "
                << other._length << ". Copying resource." << std::endl;

        std::copy(other._data, other._data + _length, _data);
    }

    // Copy assignment operator.
    MemoryBlock& operator=(const MemoryBlock& other) {
        std::cout << "Id = [" << id
                << "]. In operator=(const MemoryBlock&). length = "
                << other._length << ". Copying and assign resource."
                << std::endl;

        if (this != &other) {
            // Free the existing resource.
            delete[] _data;

            _length = other._length;
            _data = new int[_length];
            std::copy(other._data, other._data + _length, _data);
        }
        return *this;
    }

    // Retrieves the length of the data resource.
    size_t Length() const {
        return _length;
    }

    // Move constructor.
    MemoryBlock(MemoryBlock&& other) :
            _data(nullptr), _length(0) {
        std::cout << "Id = [" << id
                << "]. In MemoryBlock(MemoryBlock&&). length = "
                << other._length << ". Moving resource." << std::endl;

        // Copy the data pointer and its length from the
        // source object.
        _data = other._data;
        _length = other._length;

        // Release the data pointer from the source object so that
        // the destructor does not free the memory multiple times.
        other._data = nullptr;
        other._length = 0;
    }

    // Move assignment operator.
    MemoryBlock& operator=(MemoryBlock&& other) {
        std::cout << "Id = [" << id
                << "]. In operator=(MemoryBlock&&). length = " << other._length
                << ". Moving and assign resource." << std::endl;

        if (this != &other) {
            // Free the existing resource.
            delete[] _data;

            // Copy the data pointer and its length from the
            // source object.
            _data = other._data;
            _length = other._length;

            // Release the data pointer from the source object so that
            // the destructor does not free the memory multiple times.
            other._data = nullptr;
            other._length = 0;
        }
        return *this;
    }

private:
    size_t _length; // The length of the resource.
    int* _data; // The resource.
};

void process_value(int& i) {
    if (i == 2) {
        i = 3;
    }
    std::cout << "LValue processed: " << i << std::endl;
}

void process_value(int&& i) {
    std::cout << "RValue processed: " << i << std::endl;
}

void forward_value(int&& i) {
    process_value(i);
}

int main(int argc, char const *argv[]) {
    std::cout << "---------Left value; Right Value------------" << std::endl;
    int a = 0;
    process_value(a);
    process_value(1);
    forward_value(2);

    std::cout << "--------------------------------------------" << std::endl;
    {
        std::vector<MemoryBlock> v;
        v.push_back(MemoryBlock(10)); //存在拷贝构造、移动构造函数时，优先使用移动构造函数
    }
    //Id = [0].In MemoryBlock(size_t).length = 10.
    //Id = [1].In MemoryBlock(MemoryBlock&&).length = 10. Moving resource.
    //Id = [0].  ~MemoryBlock().length = 0.
    //Id = [1].  ~MemoryBlock().length = 10. Deleting resource.

    std::cout << "--------------------------------------------" << std::endl;
    {
        std::vector<MemoryBlock> v;
        MemoryBlock mb1(20);
        v.push_back(mb1);
    }
    //Id = [2].In MemoryBlock(size_t).length = 20.
    //Id = [3].In MemoryBlock(const MemoryBlock&).length = 20. Copying resource.
    //Id = [2].  ~MemoryBlock().length = 20. Deleting resource.
    //Id = [3].  ~MemoryBlock().length = 20. Deleting resource.

    std::cout << "--------------------------------------------" << std::endl;
    {
        std::vector<MemoryBlock> v;
        MemoryBlock mb1(30);
        v.push_back(std::move(mb1)); //不存在移动构造函数时，使用拷贝构造函数
    }
    //Id = [4].In MemoryBlock(size_t).length = 30.
    //Id = [5].In MemoryBlock(MemoryBlock&&).length = 30. Moving resource.
    //Id = [4].  ~MemoryBlock().length = 0.
    //Id = [5].  ~MemoryBlock().length = 30. Deleting resource.

    std::cout << "--------------------------------------------" << std::endl;
    {
        MemoryBlock mb1(40);
        MemoryBlock mb2(mb1); // 拷贝构造
    }
    //Id = [6].In MemoryBlock(size_t).length = 40.
    //Id = [7].In MemoryBlock(const MemoryBlock&).length = 40. Copying resource.
    //Id = [7].  ~MemoryBlock().length = 40. Deleting resource.
    //Id = [6].  ~MemoryBlock().length = 40. Deleting resource.

    std::cout << "--------------------------------------------" << std::endl;
    {
        MemoryBlock mb1(50);
        MemoryBlock mb2 = mb1; // 拷贝赋值
    }
    //Id = [8].In MemoryBlock(size_t).length = 50.
    //Id = [9].In MemoryBlock(const MemoryBlock&).length = 50. Copying resource.
    //Id = [9].  ~MemoryBlock().length = 50. Deleting resource.
    //Id = [8].  ~MemoryBlock().length = 50. Deleting resource.

    std::cout << "--------------------------------------------" << std::endl;
    {
        MemoryBlock mb1(60);
        MemoryBlock mb2 = std::move(mb1);  //移动赋值
    }
    //Id = [10].In MemoryBlock(size_t).length = 60.
    //Id = [11].In MemoryBlock(MemoryBlock&&).length = 60. Moving resource.
    //Id = [11].  ~MemoryBlock().length = 60. Deleting resource.
    //Id = [10].  ~MemoryBlock().length = 0.

    std::cout << "--------------------------------------------" << std::endl;
    {
        MemoryBlock mb1(70);
        MemoryBlock mb2(80);
        std::swap(mb1, mb2);  // 中间会有一个临时的MemoryBlock通过拷贝构造，2次拷贝赋值
    }
    //Id = [12].In MemoryBlock(size_t).length = 70.
    //Id = [13].In MemoryBlock(size_t).length = 80.
    //Id = [14].In MemoryBlock(MemoryBlock&&).length = 70. Moving resource.
    //Id = [12].In operator=(MemoryBlock&&).length = 80. Moving and assign resource.
    //Id = [13].In operator=(MemoryBlock&&).length = 70. Moving and assign resource.
    //Id = [14].  ~MemoryBlock().length = 0.
    //Id = [13].  ~MemoryBlock().length = 70. Deleting resource.
    //Id = [12].  ~MemoryBlock().length = 80. Deleting resource.

    return 0;
}
