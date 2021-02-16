#include <assert.h>
#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

namespace fortran_type {
// Types
typedef long long integer8;
typedef int integer4;
typedef short integer1;
typedef bool logical;
typedef double real8;
typedef double real;
typedef float real4;

// Design For Fortran Array
template<typename Type, int Dim>
class Farray {
  private:
    Type *base_pointer_;
    std::vector<int> dim_;

  public:
    Farray() {
        base_pointer_ = nullptr;
        dim_.clear();
    }

    Farray(std::vector<int> dim_size) {
        if (dim_size.size() != Dim) {
            std::cerr << "Dimension error!\n";
            abort();
        }

        int size_calc = 1;

        for (auto dim : dim_size) {
            #ifdef DEBUG
            size_calc *= dim;
            #endif
        }

        dim_ = dim_size;
        base_pointer_ = new Type[size_calc];
        if (base_pointer_ == nullptr) {
            std::cerr << "Memory allocation failed!\n";
            abort();
        }
    }

    ~Farray() {
        if (base_pointer_ != nullptr) {
            delete[] base_pointer_;
            base_pointer_ = nullptr;
        }
    }

    bool isValid() const {
        if (base_pointer_ == nullptr)
            return false;
        return true;
    }

    int getDim(int dimindex) {
        return dim_[dimindex - 1];
    }

    void fillArray(Type fill_data) {
        int size_calc = 1;
        for (auto dim : dim_) {
            size_calc *= dim;
        }
        std::fill_n(base_pointer_, size_calc, fill_data);
    }

    void reSize(std::vector<int> index) {
        if (base_pointer_ != nullptr) {
            delete[] base_pointer_;
        }
        int size_calc = 1;
        dim_ = index;
        for (auto dim : dim_) {
            size_calc *= dim;
        }
        base_pointer_ = new Type[size_calc];
    }

    void setValue(std::vector<int> index, Type value) {
        if (index.size() != Dim) {
            std::cerr << "Dimension error!\n";
            abort();
        }
        int index_val = 0;
        int index_level = 1;
        for (int index_index = index.size() - 1; index_index >= 0; index_index--) {
            if (index[index_index] > dim_[index_index]) {
                std::cerr << "Out of range!\n";
                abort();
            }
            index_val += (index[index_index] - 1) * index_level;
            index_level *= dim_[index_index];
        }
        &(base_pointer_ + index_val) = value;
    }

    Type &getValue(std::vector<int> index) {
        if (index.size() != Dim) {
            std::cerr << "Dimension error!\n";
            abort();
        }
        int index_val = 0;
        int index_level = 1;
        for (int index_index = index.size() - 1; index_index >= 0; index_index--) {
            if (index[index_index] > dim_[index_index]) {
                std::cerr << "Out of range!\n";
                abort();
            }
            index_val += (index[index_index] - 1) * index_level;
            index_level *= dim_[index_index];
        }
        return base_pointer_[index_val];
    }

    Type &operator()(std::vector<int> index) {
        return getValue(index);
    }
};

template<typename Type>
class Farray<Type, 1> {
  private:
    Type *base_pointer_;
    std::vector<int> dim_;

  public:
    Farray() {
        base_pointer_ = nullptr;
        dim_.clear();
    }

    Farray(int dim_size) {
        // TODO : fix for illegal size
        dim_ = std::vector<int>({dim_size});
        base_pointer_ = new Type[dim_size];
        if (base_pointer_ == nullptr) {
            std::cerr << "Memory allocation failed!\n";
            abort();
        }
    }

    Farray(std::vector<int> dim_size) {
        // cout << "Dim = " << Dim << endl;
        if (dim_size.size() != 1) {
            std::cerr << "Dimension error!\n";
            abort();
        }
        int size_calc = dim_size[0];
        dim_ = dim_size;
        base_pointer_ = new Type[size_calc];
        if (base_pointer_ == nullptr) {
            std::cerr << "Memory allocation failed!\n";
            abort();
        }
    }

    ~Farray() {
        if (base_pointer_ != nullptr) {
            delete[] base_pointer_;
            base_pointer_ = nullptr;
        }
    }

    bool isValid() const {
        if (base_pointer_ == nullptr)
            return false;
        return true;
    }

    int getDim(int dimindex) {
        return dim_[dimindex - 1];
    }

    int getSize() {
        return dim_[0];
    }

    void fillArray(Type fill_data) {
        int size_calc = dim_[0];
        std::fill_n(base_pointer_, size_calc, fill_data);
    }

    void reSize(std::vector<int> index) {
        if (base_pointer_ != nullptr) {
            delete[] base_pointer_;
        }
        dim_ = index;
        int size_calc = 1;
        for (auto dim : dim_) {
            size_calc *= dim;
        }
        base_pointer_ = new Type[size_calc];
    }

    void reSize(int length) {
        if (base_pointer_ != nullptr) {
            delete[] base_pointer_;
        }
        dim_.clear();
        dim_.push_back(length);
        base_pointer_ = new Type[length];
    }

    void setValue(std::vector<int> index, Type value) {
        if (index.size() != 1) {
            std::cerr << "Dimension error!\n";
            abort();
        }
        int index_val = index[0] - 1;
        if (index_val > dim_[0]) {
            std::cerr << "Out of range!\n";
            abort();
        }
        &(base_pointer_ + index_val) = value;
    }

    Type &getValue(std::vector<int> index) {
        if (index.size() != 1) {
            std::cerr << "Dimension error!\n";
            abort();
        }
        int index_val = index[0] - 1;
        if (index_val > dim_[0]) {
            std::cerr << "Out of range!\n";
            abort();
        }
        return base_pointer_[index_val];
    }

    Type &operator()(std::vector<int> index) {
        return getValue(index);
    }

    Farray<int, 1> &operator=(const Farray<int, 1> &array) {
        cout << "COPYING" << endl;
        exit(0);
        return *this;
    }

    void setValue(int index, Type value) {
        index--;
        &(base_pointer_ + index) = value;
    }

    Type &getValue(int index) {
        return base_pointer_[index - 1];
    }

    Type &operator()(int index) {
        return base_pointer_[index - 1];
    }

    void debugPrint(int perline) {
        int cnt = 0;
        for (int index = 0; index < dim_[0]; ++index) {
            cout << base_pointer_[index];
            cnt++;
            if (cnt % perline == 0)
                cout << endl;
            else
                cout << " ";
        }
        cout << endl;
    }

    void debugPrint() {
        int cnt = 0;
        for (int index = 0; index < dim_[0]; ++index) {
            cout << base_pointer_[index];
            cnt++;
            if (cnt % 10 == 0)
                cout << endl;
            else
                cout << " ";
        }
        cout << endl;
    }
};

template<typename Type>
class Farray<Type, 2> {
  private:
    Type *base_pointer_;
    std::vector<int> dim_;

  public:
    Farray() {
        base_pointer_ = nullptr;
        dim_.clear();
    }

    Farray(int dim_size1, int dim_size2) {
        int size_calc = dim_size1 * dim_size2;
        // TODO : fix for illegal size
        dim_ = std::vector<int>({dim_size1, dim_size2});
        // cout << size_calc << endl;
        // cout << base_pointer_ << endl;
        base_pointer_ = new Type[size_calc];
        // cout << base_pointer_ << endl;
        if (base_pointer_ == nullptr) {
            std::cerr << "Memory allocation failed!\n";
            abort();
        }
    }

    Farray(std::vector<int> dim_size) {
        // cout << "Dim = " << Dim << endl;
        if (dim_size.size() != 2) {
            std::cerr << "Dimension error!\n";
            abort();
        }
        int size_calc = 1;
        size_calc = dim_size[0] * dim_size[1];
        dim_ = dim_size;
        base_pointer_ = new Type[size_calc];
        if (base_pointer_ == nullptr) {
            std::cerr << "Memory allocation failed!\n";
            abort();
        }
    }

    ~Farray() {
        if (base_pointer_ != nullptr) {
            delete[] base_pointer_;
            base_pointer_ = nullptr;
        }
    }

    bool isValid() const {
        if (base_pointer_ == nullptr)
            return false;
        return true;
    }

    int getDim(int dimindex) {
        return dim_[dimindex - 1];
    }

    void fillArray(Type fill_data) {
        int size_calc = 1;
        for (auto dim : dim_) {
            size_calc *= dim;
        }
        std::fill_n(base_pointer_, size_calc, fill_data);
    }

    void reSize(std::vector<int> index) {
        if (base_pointer_ != nullptr) {
            delete[] base_pointer_;
        }
        dim_ = index;
        int size_calc = 1;
        for (auto dim : dim_) {
            size_calc *= dim;
        }
        base_pointer_ = new Type[size_calc];
    }

    void reSize(int dim_size1, int dim_size2) {
        if (base_pointer_ != nullptr) {
            delete[] base_pointer_;
        }
        dim_.clear();
        dim_.push_back(dim_size1);
        dim_.push_back(dim_size2);
        int size_calc = dim_size1 * dim_size2;
        base_pointer_ = new Type[size_calc];
    }

    void setValue(std::vector<int> index, Type value) {
        if (index.size() != 2) {
            std::cerr << "Dimension error!\n";
            abort();
        }
        int index_val = 0;
        int index_level = 1;
        for (int index_index = index.size() - 1; index_index >= 0; index_index--) {
            if (index[index_index] > dim_[index_index]) {
                std::cerr << "Out of range!\n";
                abort();
            }
            index_val += (index[index_index] - 1) * index_level;
            index_level *= dim_[index_index];
        }
        &(base_pointer_ + index_val) = value;
    }

    Type &getValue(std::vector<int> index) {
        if (index.size() != 2) {
            std::cerr << "Dimension error!\n";
            abort();
        }
        int index_val = 0;
        int index_level = 1;
        for (int index_index = index.size() - 1; index_index >= 0; index_index--) {
            if (index[index_index] > dim_[index_index]) {
                std::cerr << "Out of range!\n";
                abort();
            }
            index_val += (index[index_index] - 1) * index_level;
            // cout << index[index_index] << " " << index_level << endl;
            // cout << index_val << "  " << endl;
            index_level *= dim_[index_index];
        }
        // cout << "mem calc" << endl;
        // cout << base_pointer_[index_val] << " ";
        return base_pointer_[index_val];
    }

    Type &operator()(std::vector<int> index) {
        return getValue(index);
    }

    void setValue(int index1, int index2, Type value) {
        index1--;
        index2--;
        &(base_pointer_ + index1 * dim_[1] + index2) = value;
    }

    Type &getValue(int index1, int index2) {
        index1--;
        index2--;
        int index_val = index1 * dim_[1] + index2;
        return base_pointer_[index_val];
    }

    Type &operator()(int index1, int index2) {
        index1--;
        index2--;
        // cout << index1 << ", " << index2 << " -> ";
        int index_val = index1 * dim_[1] + index2;
        // cout << index_val << endl;
        return base_pointer_[index_val];
    }

    void printArray() {
        for (int index1 = 0; index1 < dim_[0]; index1++) {
            for (int index2 = 0; index2 < dim_[1]; index2++) {
                int index_val = index1 * dim_[1] + index2;
                cout << base_pointer_[index_val] << endl;
            }
        }
    }
};
}; // namespace fortran_type
