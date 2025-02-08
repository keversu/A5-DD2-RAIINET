#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <memory>

class Observer; // forward declaration

class Subject {
  std::vector<std::shared_ptr<Observer>> observers; // Shared ownership
 public:
  void attach(std::shared_ptr<Observer> o); // Accept shared_ptr
  void detach(std::shared_ptr<Observer> o); // Remove using shared_ptr
  void notifyObservers() const;
  virtual char getState(int row, int col) const = 0;
  virtual ~Subject() = default;
};

#endif
