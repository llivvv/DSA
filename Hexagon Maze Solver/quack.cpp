
/**
 * @file quack.cpp
 * Implementation of the Quack class.
 *
 */

template <class T>
Quack<T>::Quack() {
    n1 = 0;
    n2 = 0;
}

/**
 * Adds the parameter object to the right of the Quack.
 *
 * @param newItem object to be added to the Quack.
 */
template <class T>
void Quack<T>::push_right(T newItem) {
    if (n2 < int(data.size())) {
        data[n2] = newItem;
    } else {
        data.push_back(newItem);
    }
    n2++;
}

/**
 * Removes the object at the left of the Quack, and returns it to the
 * caller.
 *
 * See .h file for implementation notes.
 *
 * @return The item that used to be at the left of the Quack.
 */
template <class T>
T Quack<T>::pop_left() {
    T popped = data[n1];
    n1++;

    if (n2 - n1 <= n1) {
        vector<T> newArray;
        for (int i = n1; i < n2; i++) {
            newArray.push_back(data[i]);
        }
        n2 -= n1;
        n1 = 0;
        data = newArray;
    }

    return popped;
}
/**
 * Removes the object at the right of the Quack, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Quack.
 */
template <class T>
T Quack<T>::pop_right() {
    T popped = data[n2 - 1];
    n2--;

    if (n2 - n1 <= n1) {
        vector<T> newArray;
        for (int i = n1; i < n2; i++) {
            newArray.push_back(data[i]);
        }
        n2 -= n1; 
        n1 = 0; 
        data = newArray;
    }

    return popped;
    // T popped = data[n2 - 1];

    // if (n2 - n1 - 1 == n1) {
    //     vector<T> newArray;
    //     newArray.resize(n1);
    //     int count = 0;

    //     for (int i = n1; i < n2-2; i++) {
    //         newArray[count++] = data[i];
    //     }

    //     n2 = n1; 
    //     n1=0;
    //     data = newArray;
    // } else {
    //     n2--;
    // }

    // return popped;
}

/**
 * Finds the object at the left of the Quack, and returns it to the
 * caller. Unlike pop_left(), this operation does not alter the quack.
 *
 * @return The item at the front of the quack.
 */
template <class T>
T Quack<T>::peekL() {
    return data[n1];
}

/**
 * Finds the object at the right of the Quack, and returns it to the
 * caller. Unlike pop_right(), this operation does not alter the quack.
 *
 * @return the value of The item at the right of the quack.
 */
template <class T>
T Quack<T>::peekR() {
    return data[n2 - 1];
}

/**
 * Determines if the Quack is empty.
 *
 * @return bool which is true if the Quack is empty, false otherwise.
 */
template <class T>
bool Quack<T>::is_empty() const {
    return n1 == n2;
} 

