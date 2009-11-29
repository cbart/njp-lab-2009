/** Quaternion Sequences.                                  *
 * author: Cezary Bartoszuk <cbart@students.mimuw.edu.pl>  *
 *     id: cb277617@students.mimuw.edu.pl                  */

#ifndef _QUATSEC_H_
#define _QUATSEC_H_

#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>

#include "quaternion.h"
#include "debug_tools.h"
#include "safe_bool.h"

class QuaternionSequence : public SafeBool<> {
    /** Sequences of quaternions. */

    public:

        typedef unsigned long size_type;

        class ProxyQContainer : public IQContainer {

            public:

                friend class QuaternionSequence;

            protected:

                 size_type index;
                 QuaternionSequence* qs;
                 ProxyQContainer(size_type id, QuaternionSequence& qs);
                 ProxyQContainer(const ProxyQContainer& cnt);

            public:

                 ProxyQContainer& operator=(const ProxyQContainer& cnt);
                 virtual ~ProxyQContainer();
                 virtual ProxyQContainer* clone() const; /* @Override */
                 virtual Real getA() const;              /* @Override */
                 virtual Real getB() const;              /* @Override */
                 virtual Real getC() const;              /* @Override */
                 virtual Real getD() const;              /* @Override */
                 virtual void setA(Real a);              /* @Override */
                 virtual void setB(Real b);              /* @Override */
                 virtual void setC(Real c);              /* @Override */
                 virtual void setD(Real d);              /* @Override */

        };

    private:

        std::map<size_type, Quaternion> seq;

    protected:

        /* Returns quaternion with given id from the map.
         * If quaternion does not exist -> creates zero quaternion
         * and returns it. */
        Quaternion& getQuaternion(size_type id);

        /* Copies `Quaternion` `q` to index `id`. */
        Quaternion& setQuaternion(size_type id, const Quaternion& q);

        /* Test for if(QuaternionSequence); */
        bool booleanTest() const;

    public:

        /** Creates sequence which all elements are 0. */
        QuaternionSequence();

        /** Creates sequence which elements are defined in given `map`. */
        explicit QuaternionSequence
            (const std::map<size_type, Quaternion>& m_seq);

        /** Creates sequence which first elements
         *  are defined in given `vector`. */
        explicit QuaternionSequence(const std::vector<Quaternion>& v_seq);

        /** Creates a copy of given `QuaternionSequence` `qs`. */
        QuaternionSequence(const QuaternionSequence& qs);

        /** Destroys `self` quaternion sequence. */
        virtual ~QuaternionSequence();

        /** Assigns given `qs` to `self`. */
        QuaternionSequence& operator=(const QuaternionSequence& qs);

        /** Adds given `qs` to `self`. */
        QuaternionSequence& operator+=(const QuaternionSequence& qs);

        /** Subtracts `qs` from `self`. */
        QuaternionSequence& operator-=(const QuaternionSequence& qs);

        /** Multiplies `self` by `qs`. */
        QuaternionSequence& operator*=(const QuaternionSequence& qs);

        /** Multiplies `self` by quaternion `q`. */
        QuaternionSequence& operator*=(const Quaternion& q);

        /** Divides `self` by quaternion `q`. */
        QuaternionSequence& operator/=(const Quaternion& q)
            throw (DivideByZeroException);

        /** Returns quaternion of index `id`. */
        const Quaternion& operator[](size_type id) const;

        /** Returns non const quaternion of index `id`.
         *  Used for assignment. */
        Quaternion operator[](size_type id);

        /** Treats this quaternion sequence as a power series
         *  and calculates value in `q`. */
        const Quaternion operator()(const Quaternion& q) const;

        /** Returns new quaternion sequence created
         *  by adding two given sequences. */
        friend const QuaternionSequence operator+
            (const QuaternionSequence& qs1, const QuaternionSequence& qs2);

        /** Returns new quaternion sequence created
         *  by subtracting `qs2` from `qs1`. */
        friend const QuaternionSequence operator-
            (const QuaternionSequence& qs1, const QuaternionSequence& qs2);

        /** Returns new quaternion sequence created
         *  by multiplying `qs` by `qs2`. */
        friend const QuaternionSequence operator*
            (const QuaternionSequence& qs1, const QuaternionSequence& qs2);

        /** Returns new quaternion sequence created
         *  by multiplying sequence `qs` by quaternion `q`. */
        friend const QuaternionSequence operator*
            (const QuaternionSequence& qs, const Quaternion& q);

        /** Returns new quaternion sequence created
         *  by multiplying quaternion `q` by sequence `qs`. */
        friend const QuaternionSequence operator*
            (const Quaternion& q, const QuaternionSequence& qs);

        /** Returns new quaternion sequence created
         *  by dividing sequence `qs` by quaternion `q`. */
        friend const QuaternionSequence operator/
            (const QuaternionSequence& qs, const Quaternion& q);

        /** Indicates if two quaternion sequences are equal. */
        friend bool operator==
            (const QuaternionSequence& qs1, const QuaternionSequence& qs2);

        /** Indicates whether two quaternion sequences vary. */
        friend bool operator!=
            (const QuaternionSequence& qs1, const QuaternionSequence& qs2);

        /** Printing the sequence. */
        friend std::ostream& operator<<
            (std::ostream& os, const QuaternionSequence& qs);
};

#endif
