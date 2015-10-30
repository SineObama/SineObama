/*
 * EncryptionStorage.h
 *
 *  Created on: 2015Äê6ÔÂ10ÈÕ
 *      Author: Sine
 */

// strangely encrypt string to number, more than 3 times longer...
// it's just a wild thought

#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

#include <string>
#include <fstream>

const int LEN = 3;  // length double times, no less than 3

class Encryption {

 public:

    std::string encrypt(const std::string &);
    std::string decrypt(const std::string &);

 private:

    std::string basicencrypt2(const std::string &);
    std::string basicdecrypt2(const std::string &);

    std::string basicencrypt(const std::string &);
    std::string basicdecrypt(const std::string &);

};

#endif /* ENCRYPTION_H_ */
