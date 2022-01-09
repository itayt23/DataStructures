#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>


class Exception: public std::exception{};

class AllocationError : public std::bad_alloc {};
class InvalidArg : public std::exception {};
class NodeAlreadyExist : public std::exception {};
class NodeDosentExist : public std::exception {};
class Failure : public std::exception {};
class Assert : public std::exception {};

#endif  /* EXCEPTIONS_H_ */
