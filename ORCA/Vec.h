//
//  Vec.h
//  ORCAMath
//
//  Created by Daniel Pietz on 8/15/20.
//  Copyright © 2020 Daniel Pietz. All rights reserved.
//  Version 1.0 Updated August 15, 2020
//

#ifndef Vec_h
#define Vec_h

/* Includes for Vec.h */

#include "Except.h" // Included for ORCA Exceptions
#include "Mat.h"    // Included for Matrix subclassing

namespace ORCA {

/**
 * Vector class
 */

template<class T>
class Vec : public Mat<T> {
private:
protected:
    
    /* Below are protected variables for the Vec class */
    
    index_t _n_elems;       // Stores the number of elements in the vector
    
    /* Below are protected constructors for the Vec class */
    
    /**
     * Default constructor for Vec class.
     */
    
    Vec() {
        
    } /* Vec() */
    
    /* Below are protected member functions for the Vec class */
    
    /**
     * Allocates space for vector
     * @param n_elems Number of elements to allocate space for
     */
    
    virtual void _allocate(index_t n_elems) {
#ifndef ORCA_DISABLE_EMPTY_CHECKS
        if (_n_elems < 0) {
            throw ORCAExcept::EmptyElementError(); // Attempting to allocate an empty vector
        }
#endif
        this->_mat = static_cast<T*>(malloc(sizeof(T) * n_elems));
        this->_n_elems = n_elems;
        this->_n_cols = n_elems;
        this->_n_rows = 1;
    } /* virtual void _allocate(index_t _n_elems) */
    
    /**
     * Returns the address of the requsted element
     * @param elem Index of element
     */
    
    virtual T* _address(index_t elem) const {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if ((elem < 0) || (elem > this->length())) {
            throw ORCAExcept::OutOfBoundsError(); // Attempting to index out of bounds
        }
#endif
        return this->_mat + elem;
    } /* virtual T* _address(index_t elem) const */
    
public:
    
    /* Below are public constructors for the Vec class */
    
    /**
     * Casted constructor from another vector
     * @tparam T1 class of other vector
     * @param _casted other vector
     */
    template <class T1>
    Vec(Vec<T1> _casted) {
        this->_allocate(_casted.length());
        index_t i;
        for (i = 0; i < _casted.length(); ++i) {
            this->set(i, _casted.at(i));
        }
    }
    
    /**
     * Casted constructor from pointer to another vector
     * @tparam T1 class of other vector
     * @param _casted pointer to other vector
     */
    template <class T1>
    Vec(Vec<T1>* _casted) {
        this->_allocate(_casted->length());
        index_t i;
        for (i = 0; i < _casted->length(); ++i) {
            this->set(i, _casted->at(i));
        }
    }
    
    /**
     * Constructs a vector with the specified number of elements
     * @param elements number of elements in vector
     */
    
    Vec(index_t elements) : Mat<T>(1, elements) {
        this->_allocate(elements);
    } /* Vec(index_t elements) */
    
    /**
     * Constructs a vector from an initializer list
     * @param _casted_values values to cast to vector
     */
    
    Vec(std::initializer_list<T> _casted_values) {
        this->_allocate(_casted_values.size());
        index_t i;
        for (i = 0; i < this->_n_elems; ++i) {
            this->set(i, *(_casted_values.begin() + i));
        }
    } /* Vec(std::initializer_list<T> _casted_values) */
    
    /* Below are public setters for the Vec class */
    
    /**
     Assigns an element at the speciifed index.
     * @param index Element  Index
     * @param elem Element
     */
    
    virtual void set(index_t index, T elem) {
        /* Assign Element */
        *(this->_address(index)) = elem;
        /* Reset Sticky Compute Mask */
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        this->_stickyComputeMask = 0;
#endif
    } /* virtual void set(index_t index, T elem) */
    
    /**
     Assigns an element at the speciifed index.
     * @param row Element Row Index
     * @param col Element Column Index
     * @param elem Element
     */
    
    virtual void set(index_t row, index_t col, T elem) override {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        /* Check for out of bounds indexing */
        if (row != 0) {
            throw ORCAExcept::OutOfBoundsError(); // Indexed past first row of vector
        }
#endif
        /* Assign Element */
        *(this->_address(col)) = elem;
        /* Reset Sticky Compute Mask */
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        this->_stickyComputeMask = 0;
#endif
    } /* virtual void set(index_t row, index_t col, T elem) override */
    
    /* Below are public getters for the Vec class */
    
    /**
     * Returns element at the specified index
     * @param index Element  Index
     * @return element at index
     */
    
    virtual T at(index_t index) const {
        return *(this->_address(index)); //Index the matrix
    } /* virtual T at(index_t index) const */
    
    /**
     * Returns element at the specified index
     * @param row Element Row Index
     * @param col Element Column Index
     * @return element at index
     */
    
    virtual T at(index_t row, index_t col) const override {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        /* Check for out of bounds indexing */
        if (row != 0) {
            throw ORCAExcept::OutOfBoundsError(); // Indexed past first row of vector
        }
#endif
        return *(this->_address(col)); //Index the matrix
    } /* virtual T at(index_t row, index_t col) const override */
    
    index_t length() const {
        return this->_n_elems;
    } /* index_t length() const */
    
    /**
     * Returns the sum of elements in the vector
     * @return sum of vector elements
     */
    
    T sum() const {
        index_t i;
        T result = 0;
        for (i = 0; i < this->length(); ++i) {
            result += this->at(i);
        }
        return result;
    } /* T sum() const */
    
    /**
     * Returns the product of elements in the vector
     * @return product of vector elements
     */
    
    T prod() const {
        index_t i;
        T result = 1;
        for (i = 0; i < this->length(); ++i) {
            result *= this->at(i);
        }
        return result;
    } /* T sum() const */
    
}; /* Vec class */

/* Below are overloaded operations for the Vector class */

/**
 * Returns the sum of elements in the vector
 * @param _vec Vector
 * @return sum of vector elements
 */

template <class T>
T sum(Vec<T> _vec) {
    return _vec.sum();
} /* T sum(Vec<T> _vec) */

/**
 * Returns the product of elements in the vector
 * @param _vec Vector
 * @return product of vector elements
 */

template <class T>
T prod(Vec<T> _vec) {
    return _vec.prod();
} /* T prod(Vec<T> _vec) */

/**
 * RowVec
 */

template <class T>
class RowVec : public Vec<T> {
protected:
    
    /* Below are the protected constructors for the RowVec class */
    
    RowVec() {
        
    } /* RowVec() */
    
public:
    
    /* Below are the public constructors for the RowVec class */
    
    /**
     * Constructs empty vector with specified number of elements
     * @param elements length of vector
     */
    
    RowVec(int elements) {
        this->_allocate(elements);
    } /* RowVec(int elements) */
    
    /**
     * Constructs a vector from an initializer list
     * @param _casted_values values to cast to vector
     */
    
    RowVec(std::initializer_list<T> _casted_values) : Vec<T>(_casted_values) {
    } /* RowVec(std::initializer_list<T> _casted_values) */
    
    /**
     * Casted constructor from   another vector
     * @tparam T1 class of other vector
     * @param _casted pointer to other vector
     */
    template <class T1>
    RowVec(Vec<T1> _casted) {
        this->_allocate(_casted.length());
        index_t i;
        for (i = 0; i < _casted.length(); ++i) {
            this->set(i, _casted.at(i));
        }
    }
    
    /**
     * Casted constructor from pointer to another vector
     * @tparam T1 class of other vector
     * @param _casted pointer to other vector
     */
    template <class T1>
    RowVec(Vec<T1>* _casted) {
        this->_allocate(_casted->length());
        index_t i;
        for (i = 0; i < _casted->length(); ++i) {
            this->set(i, _casted->at(i));
        }
    }
    
    /**
     * Casted constructor from MatRow type
     * @tparam T1 class of other vector
     * @param _casted pointer to other vector
     */
    template <class T1>
    RowVec(typename Mat<T1>::MatRow _casted) {
        this->_allocate(_casted.length());
        index_t i;
        for (i = 0; i < _casted.length(); ++i) {
            this->set(i, _casted.at(i));
        }
    }
    
    
};

/**
 * ColVec
 */

template <class T>
class ColVec : public Vec<T> {
protected:
    
    /* Below are protected member functions for the ColVec class */
    
    /**
     * Allocates space for vector
     * @param _n_elems Number of elements to allocate space for
     */
    
    virtual void _allocate(index_t _n_elems) override {
#ifndef ORCA_DISABLE_EMPTY_CHECKS
        if (_n_elems < 0) {
            throw ORCAExcept::EmptyElementError(); // Attempting to allocate an empty vector
        }
#endif
        this->_mat = static_cast<T*>(malloc(sizeof(T) * _n_elems));
        this->_n_elems = _n_elems;
        this->_n_cols = 1;
        this->_n_rows = _n_elems;
    } /* virtual void _allocate(index_t _n_elems) */
    
    /* Below are the protected constructors for the RowVec class */
    
    ColVec() {
        
    } /* ColVec() */
    
public:
    
    /* Below are the public destructors for the RowVec class */
    
    ~ColVec() {
        //TODO: Pointer being freed was not allocated error
        //delete this->_vec;
    } /* ~RowVec() */
    
    /* Below are the public constructors for the RowVec class */
    
    /**
     * Constructs empty vector with specified number of elements
     * @param elements length of vector
     */
    
    ColVec(int elements) {
        this->_allocate(elements);
    } /* RowVec(int elements) */
    
    /**
     * Constructs a vector from an initializer list
     * @param _casted_values values to cast to vector
     */
    
    ColVec(std::initializer_list<T> _casted_values) : Vec<T>(_casted_values){
    } /* ColVec(std::initializer_list<T> _casted_values) */
    
    /**
     * Casted constructor from   another vector
     * @tparam T1 class of other vector
     * @param _casted pointer to other vector
     */
    template <class T1>
    ColVec(Vec<T1> _casted) {
        this->_allocate(_casted.length());
        index_t i;
        for (i = 0; i < _casted.length(); ++i) {
            this->set(i, _casted.at(i));
        }
    }
    
    /**
     * Casted constructor from pointer to another vector
     * @tparam T1 class of other vector
     * @param _casted pointer to other vector
     */
    template <class T1>
    ColVec(Vec<T1>* _casted) {
        this->_allocate(_casted->length());
        index_t i;
        for (i = 0; i < _casted->length(); ++i) {
            this->set(i, _casted->at(i));
        }
    }
    
    /* Below are the public setters for the ColVec class */
    
    /**
     Assigns an element at the speciifed index.
     * @param index Element  Index
     * @param elem Element
     */
    
    virtual void set(index_t index, T elem) override {
        /* Assign Element */
        *(this->_address(index)) = elem;
        /* Reset Sticky Compute Mask */
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        this->_stickyComputeMask = 0;
#endif
    } /* virtual void set(index_t index, T elem) */
    
    
    /**
     Assigns an element at the speciifed index.
     * @param row Element Row Index
     * @param col Element Column Index
     * @param elem Element
     */
    
    virtual void set(index_t row, index_t col, T elem) override {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        /* Check for out of bounds indexing */
        if (col != 0) {
            throw ORCAExcept::OutOfBoundsError(); // Indexed past first row of vector
        }
#endif
        /* Assign Element */
        *(this->_address(row)) = elem;
        /* Reset Sticky Compute Mask */
#ifndef ORCA_DISABLE_STICKY_COMPUTE
        this->_stickyComputeMask = 0;
#endif
    } /* virtual void set(index_t row, index_t col, T elem) override */
    
    /* Below are the public getters for the ColVec class */
    
    /**
     * Returns element at the specified index
     * @param index Element  Index
     * @return element at index
     */
    
    virtual T at(index_t index) const override {
        return *(this->_address(index)); //Index the matrix
    } /* virtual T at(index_t index) const */
    
    /**
     * Returns element at the specified index
     * @param row Element Row Index
     * @param col Element Column Index
     * @return element at index
     */
    
    /**
     * Returns element at the specified index
     * @param row Element Row Index
     * @param col Element Column Index
     * @return element at index
     */
    
    virtual T at(index_t row, index_t col) const override {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        /* Check for out of bounds indexing */
        if (col != 0) {
            throw ORCAExcept::OutOfBoundsError(); // Indexed past first row of vector
        }
#endif
        return *(this->_address(row)); //Index the matrix
    } /* virtual T at(index_t row, index_t col) const override */
    
};

/* Below are nonmember functions for the Vector class */
template <class T1, class T2>
auto dot(Vec<T1> v1, Vec<T2> v2) {
    //TODO: Return types are not type safe.
#ifndef ORCA_DISABLE_EMPTY_CHECKS
    if (v1.length() == 0 || v2.length() == 0) {
        throw ORCAExcept::EmptyElementError(); // Attempting to dot an empty vector
    }
#endif
    
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
    if (v1.length() != v2.length()) {
        throw ORCAExcept::BadDimensionsError(); // Attempting to dot 2 vectors with incompatible dimensions
    }
#endif
    auto result = v1.at(0) * v2.at(0);
    index_t i;
    for (i = 1; i < v1.length(); ++i) {
        result += v1.at(i) * v2.at(i);
    }
    return result;
} /* auto dot(Vec<T1> v1, Vec<T2> v2) */



/* MatRow Class: Vector that points to a specific row in a matrix */

template <class T>
class Mat<T>::MatRow : public RowVec<T> {
    /* Below are the private member variables for the MatRow class */
    Mat<T>* _matrix;        // Stores a pointer to the matrix the row belongs to
    index_t _row;               // Stores the row number in _matrix
public:
    
    /* Below are all public constructors for the MatRow class */
    
    /**
     * Value constructor for MatRow
     * @param matrix Matrix to obtain row from
     * @param row Row number
     */
    
    MatRow(Mat<T> matrix, index_t row) {
        this->_matrix = &matrix;
        this->_row = row;
        this->_n_elems = matrix.cols();
        this->_n_cols = matrix.cols();
        this->_n_rows = 1;
    }
    
    /**
     * Pointer constructor for MatRow
     * @param matrix pointer to Matrix to obtain row from
     * @param row Row number
     */
    
    MatRow(Mat<T>* matrix, index_t row) {
        this->_matrix = matrix;
        this->_row = row;
        this->_n_elems = matrix->cols();
        this->_n_cols = matrix->cols();
        this->_n_rows = 1;
    }
    /* Below are public getters for the MatRow class */
    
    /**
     * Returns element at the specified index
     * @param index Element  Index
     */
    
    virtual T at(index_t index) const override {
        return this->_matrix->at(this->_row, index); //Index the matrix
    }
    
    /**
     * Returns element at the specified index
     * @param row Element Row Index
     * @param col Element Column Index
     */
    
    virtual T at(index_t row, index_t col) const override {
        return this->_matrix->at(this->_row, col); //Index the matrix
    }
    
};

/* MatCol Class: Vector that points to a specific column in a matrix */

template <class T>
class Mat<T>::MatCol : public ColVec<T> {
    /* Below are the private member variables for the MatRow class */
    Mat<T>* _matrix;        // Stores a pointer to the matrix the row belongs to
    index_t _col;               // Stores the col number in _matrix
public:
    
    /* Below are all public constructors for the MatCol class */
    
    /**
     * Value constructor for MatCol
     * @param matrix Matrix to obtain column from
     * @param col Column number
     */
    
    MatCol(Mat<T> matrix, index_t col) {
        this->_matrix = &matrix;
        this->_col = col;
        this->_n_elems = matrix.rows();
        this->_n_cols = 1;
        this->_n_rows = matrix.cols();
    }
    
    /**
     * Pointer constructor for MatCol
     * @param matrix Pointer to mMatrix to obtain column from
     * @param col Column number
     */
    
    MatCol(Mat<T> *matrix, index_t col) {
        this->_matrix = matrix;
        this->_col = col;
        this->_n_elems = matrix->rows();
        this->_n_cols = 1;
        this->_n_rows = matrix->rows();
    }
    
    /**
     * Returns element at the specified index
     * @param index Element  Index
     */
    
    virtual T at(index_t index) const override {
        return this->_matrix->at(index, this->_col); //Index the matrix
    }
    
    /**
     * Returns element at the specified index
     * @param row Element Row Index
     * @param col Element Column Index
     */
    
    virtual T at(index_t row, index_t col) const override {
#ifndef ORCA_DISABLE_BOUNDS_CHECKS
        if (col != this->_col) {
            throw ORCAExcept::OutOfBoundsError(); // Indexed outside col bounds
        }
#endif
        return this->_matrix->at(this->_col, row); //Index the matrix
    }
    
};

}

#endif /* Vec_h */
