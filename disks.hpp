///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum disk_color { DISK_LIGHT, DISK_DARK };

class disk_state {
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count) : _colors(light_count * 2, DISK_LIGHT) {

    assert(light_count > 0);

    for (size_t i = 1; i < _colors.size(); i += 2) {
      _colors[i] = DISK_DARK;
    }
  }

  bool operator==(const disk_state &rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const { return _colors.size(); }

  size_t light_count() const { return total_count() / 2; }

  size_t dark_count() const { return light_count(); }

  bool is_index(size_t i) const { return (i < total_count()); }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_initialized() const {
    for (size_t i = 0; i < total_count();
         i++) {         // check each position before function
      if (i % 2 == 0) { // check even position --> should be light
        if (_colors[i] == DISK_DARK) {
          return false;
        }
      } else { // check odd position --> should be dark
        if (_colors[i] == DISK_LIGHT) {
          return false;
        }
      }
    }

    return true;
  }

  // @Breif: Return true when this disk_state is fully sorted, with all light disks on
  // the left (low indices) and all dark disks on the right (high indices).
  bool is_sorted() const {
    // loop through the total_count()
    for (size_t i = 0; i < total_count(); i++) {
      if (i < total_count() / 2) {
        if (_colors[i] == DISK_DARK) {

          return false;
        }
      } else {
        if (_colors[i] == DISK_LIGHT) {
          return false;
        }
      }
    }
    return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:
  sorted_disks(const disk_state &after, unsigned swap_count)
      : _after(after), _swap_count(swap_count) {}

  sorted_disks(disk_state &&after, unsigned swap_count)
      : _after(after), _swap_count(swap_count) {}

  const disk_state &after() const { return _after; }

  unsigned swap_count() const { return _swap_count; }
};

// @Breif:starts with the leftmost disk and proceeds to the right until it reaches the rightmost disk: 
// compares every two adjacent disks and swaps them only if necessary. 
// It does not iterate through each index, but iterates over each pair (i.e., it moves 2 steps at a time). 
// We consider a run to be a check of adjacent disks from left-to-right.
sorted_disks sort_alternate(const disk_state &before) { // record # of step swap
                                                                                  //Step Count
  int numOfSwap = 0;                                                              // 1 times
  disk_state step = before;                                                       // 1 times
                                                                             
  for (size_t i = 0; i < step.total_count(); i++) {                               // n + 1 times
    for (size_t j = 0; j < step.total_count() - 1; j++) {                         // (n-1+1) -> n times
      if (step.get(j) > step.get(j + 1)) {                                        // 2 times
        step.swap(j);                                                             // 1 times
        numOfSwap++;                                                              // 1 times
      }
    }
 }
  return sorted_disks(disk_state(step), numOfSwap);                               // 0 times
}

// @Breif:compares every two adjacent disks and proceeds to the left until it reaches the leftmost disk,
// doing the swaps only if necessary. The lawnmower movement is repeated ⌈n/2⌉ times. 
sorted_disks sort_lawnmower(const disk_state &before) {
                                                                                  //Step Count
  int numOfSwap = 0;                                                              // 1 time unit        
  disk_state step = before;                                                       // 1 time unit 

    for(int i = 0; i < step.light_count(); i++) {                                   //(n+1/2) times
 // left to right - compares every two adjacent disks and swaps if necessary  (i%2 == 0)- meaning it is even                                                    
      for(size_t j = 0; j < step.total_count() - 1; j++) {                        // n - 1 + 1 -> n times unit 
          if (step.get(j) > step.get(j + 1)) {                                    // 2 time unit 
            step.swap(j);                                                         // 1 time unit 
            numOfSwap++;                                                          // 1 time unit 
          }
      }
    } // else it is odd
    //right to left - compares every two adjacent disks and swaps if necessary
      for (size_t k = step.total_count() - 1; k > 0; k--) {                       // n - 1 + 1 -> n time unit 
          if (step.get(k) < step.get(k - 1)) {                                    // 2 times unit                           
            step.swap(k - 1);                                                     // 2 times unit                  
            numOfSwap++;                                                          // 1 times unit
          }
      }
    
  return sorted_disks(disk_state(step), numOfSwap);                               // 0 times
}
