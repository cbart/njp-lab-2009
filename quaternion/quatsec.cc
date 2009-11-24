#include <iostream>
#include <stdexcept>
#include <map>
#include <deque>

#include "./debug_tools.h"
#include "./quatsec.h"
#include "./quaternion.h"

#ifdef DEBUGLEVEL
    const DebugLevel DEBUG_LEVEL = DEBUGLEVEL;
#else
    const DebugLevel DEBUG_LEVEL = 0;
#endif

typedef QuaternionSequence::ProxyQContainer ProxyQContainer;
typedef QuaternionSequence::size_type size_type;

const String DIAG_PROG_NAME = "Quatsec";
const DebugLevel ERROR_DEBUG_LEVEL = 2;
const DebugLevel WARNING_DEBUG_LEVEL = 2;
const DebugLevel INFO_DEBUG_LEVEL = 2;

ProxyQContainer::ProxyQContainer
    (QuaternionSequence::size_type id,
     QuaternionSequence& qs)
  : index(id),
    qs(&qs)
{
    logInfo() << "QuaternionSequece::ProxyQContainer::"
        "ProxyQContainer(QuaternionSequence::size_type id);\n" << std::flush;
}

ProxyQContainer::ProxyQContainer(const QuaternionSequence::ProxyQContainer& cnt)
  : index(cnt.index),
    qs(cnt.qs)
{
    logInfo() << "QuaternionSequence::ProxyQContainer::"
        "ProxyQContainer(QuaternionSequence::ProxyQContainer& cnt);\n"
        << std::flush;
}

ProxyQContainer& ProxyQContainer::operator=
    (const ProxyQContainer& cnt)
{
    index = cnt.index;
    qs = cnt.qs;
    return *this;
}

ProxyQContainer::~ProxyQContainer()
{
    logInfo() << "QuaternionSequece::ProxyQContainer::~ProxyQContainer();\n"
        << std::flush;
}

ProxyQContainer* ProxyQContainer::clone() const
{
    logInfo() << "QuaternionSequence::ProxyQContainer::clone();\n"
        << std::flush;
    return new ProxyQContainer(index, *qs);
}

Real ProxyQContainer::getA() const
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.r();
    else
        return RZERO;
}

Real ProxyQContainer::getB() const
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.i();
    else
        return RZERO;
}

Real ProxyQContainer::getC() const
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.j();
    else
        return RZERO;
}

Real ProxyQContainer::getD() const
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.k();
    else
        return RZERO;
}

void ProxyQContainer::setA(Real a)
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setA(a);
}

void ProxyQContainer::setB(Real b)
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setB(b);
}

void ProxyQContainer::setC(Real c)
{
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setC(c);
}

void ProxyQContainer::setD(Real d)
{
    if(qs->seq.find(index) == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setD(d);
}

Quaternion& QuaternionSequence::getQuaternion(size_type id)
{
    if(seq.find(id) == seq.end())
        seq[id] = Quaternion();
    return seq[id];
}

Quaternion& QuaternionSequence::setQuaternion
    (size_type id, const Quaternion& q)
{
    logInfo() << "QuaternionSequence::setQuaternion"
        "(size_type id, Quaternion q);\n"
        "\tCalled with (id): " << id << ",\n"
        "\tCalled with (q):  " << q << ".\n" << std::flush;
    seq[id] = Quaternion(q);
    return seq[id];
}

QuaternionSequence::QuaternionSequence()
  : seq(std::map<size_type, Quaternion>())
{
    logInfo() << "QuaternionSequence::QuaternionSequence();\n"
        << std::flush;
}

QuaternionSequence::QuaternionSequence
    (const std::map<size_type, Quaternion>& m_seq)
  : seq(m_seq)
{
    logInfo() << "QuaternionSequence::QuaternionSequence"
        "(const std::map<QuaternionSequence::size_type, Quaternion>);\n"
        << std::flush;
}

QuaternionSequence::QuaternionSequence
    (const std::vector<Quaternion>& v_seq)
  : seq(std::map<size_type, Quaternion>())
{
    logInfo() << "QuaternionSequence::QuaternionSequence"
        "(const std::vector<Quaternion>& v_seq);\n" << std::flush;
    size_type i = 0;
    for(std::vector<Quaternion>::const_iterator iter = v_seq.begin();
            iter < v_seq.end();
            iter ++)
        seq[i ++] = *iter;
}

QuaternionSequence::QuaternionSequence
    (const QuaternionSequence& qs)
  : seq(std::map<size_type, Quaternion>(qs.seq))
{
    logInfo() << "QuaternionSequence::QuaternionSequence"
        "(const QuaternionSequence& qs);\n" << std::flush;
}

QuaternionSequence::~QuaternionSequence()
{
    logInfo() << "QuaternionSequence::~QuaternionSequence();\n" << std::flush;
}

QuaternionSequence& QuaternionSequence::operator=
(const QuaternionSequence& qs)
{
    logInfo() << "QuaternionSequence& QuaternionSequence::operator="
        "(const QuaternionSequence& qs);\n" << std::flush;
    seq = qs.seq;
    return *this;
}

QuaternionSequence& QuaternionSequence::operator+=
(const QuaternionSequence& qs)
{
    logInfo() << "QuaternionSequence& QuaternionSequence::operator+="
        "(const QuaternionSequence& qs);\n" << std::flush;
    for(std::map<size_type, Quaternion>::const_iterator i = qs.seq.begin();
            i != qs.seq.end();
            i ++) {
        std::map<size_type, Quaternion>::iterator finder = seq.find(i->first);
        if(finder != seq.end())
            seq[i->first] += i->second;
        else
            seq[i->first] = i->second;
    }
    return *this;
}

QuaternionSequence& QuaternionSequence::operator-=
(const QuaternionSequence& qs)
{
    logInfo() << "QuaternionSequence& QuaternionSequence::operator-="
        "(const QuaternionSequence& qs);\n" << std::flush;
    for(std::map<size_type, Quaternion>::const_iterator i = qs.seq.begin();
            i != qs.seq.end();
            i ++) {
        std::map<size_type, Quaternion>::iterator finder = seq.find(i->first);
        if(finder != seq.end())
            seq[i->first] -= i->second;
        else
            seq[i->first] = -(i->second);
    }
    return *this;
}

QuaternionSequence& QuaternionSequence::operator*=
(const Quaternion& q)
{
    logInfo() << "QuaternionSequence& QuaternionSequence::operator*="
        "(const Quaternion& q);\n" << std::flush;
    for(std::map<size_type, Quaternion>::iterator i = seq.begin();
            i != seq.end();
            i ++) {
        seq[i->first] *= q;
    }
    return *this;
}

QuaternionSequence& QuaternionSequence::operator/=
(const Quaternion& q)
    throw (DivideByZeroException)
{
    logInfo() << "QuaternionSequence& QuaternionSequence::operator/="
        "(const Quaternion& q);\n" << std::flush;
    for(std::map<size_type, Quaternion>::iterator i = seq.begin();
            i != seq.end();
            i ++) {
        seq[i->first] /= q;
    }
    return *this;
}

QuaternionSequence& QuaternionSequence::operator*=
(const QuaternionSequence& qs)
{
    logInfo() << "QuaternionSequence& QuaternionSequence::operator*="
        "(const QuaternionSequence& qs);\n" << std::flush;
    std::map<size_type, Quaternion>::const_iterator qs_iter = qs.seq.begin();
    std::map<size_type, Quaternion>::iterator self_iter = seq.begin();
    std::deque<size_type> to_erase = std::deque<size_type>();
    while(qs_iter != qs.seq.end() && self_iter != seq.end()) {
        size_type self_pos = self_iter->first;
        size_type qs_pos = qs_iter->first;
        if(self_pos < qs_pos) {
            to_erase.push_back(self_pos);
            self_iter ++;
        }
        else if(self_pos == qs_pos) {
            seq[self_pos] *= qs_iter->second;
            self_iter ++;
            qs_iter ++;
        }
        else { /* if self_pos > qs_pos */
            qs_iter ++;
        }
    }
    while(self_iter != seq.end()) {
        to_erase.push_back(self_iter->first);
        self_iter ++;
    }
    while(!to_erase.empty()) {
        seq.erase(to_erase.front());
        to_erase.pop_front();
    }
    return *this;
}

const Quaternion& QuaternionSequence::operator[](size_type id) const
{
    static Quaternion zero_quat = Quaternion();
    logInfo() << "const Quaternion& QuaternionSequence::operator[]"
        "(QuaternionSequence::size_type id) const;\n" << std::flush;
    std::map<size_type, Quaternion>::const_iterator iter = seq.find(id);
    if(iter != seq.end())
        return iter->second;
    else
        return zero_quat;
}

Quaternion QuaternionSequence::operator[](size_type id)
{
    logInfo() << "Quaternion QuaternionSequence::operator[]"
        "(QuaternionSequence::size_type id);\n" << std::flush;
    return Quaternion(new ProxyQContainer(id, *this));
}

const Quaternion QuaternionSequence::operator()(const Quaternion& q) const
{
    logInfo() << "Quaternion QuaternionSequence::operator[]"
        "(const Quaternion& q) const;\n" << std::flush;
    Quaternion sum = Quaternion();
    std::map<size_type, Quaternion>::const_reverse_iterator iter = seq.rbegin();
   //TODO: schemat Hornera
    while(iter != seq.rend()) {
        sum += (iter->second * pow(q, iter->first));
    }
    return sum;
}

const QuaternionSequence operator+
(const QuaternionSequence& qs1, const QuaternionSequence& qs2)
{
    return QuaternionSequence(qs1) += qs2;
}

const QuaternionSequence operator-
(const QuaternionSequence& qs1, const QuaternionSequence& qs2)
{
    return QuaternionSequence(qs1) -= qs2;
}

const QuaternionSequence operator*
(const QuaternionSequence& qs1, const QuaternionSequence& qs2)
{
    return QuaternionSequence(qs1) *= qs2;
}

const QuaternionSequence operator*
(const QuaternionSequence& qs, const Quaternion& q)
{
    return QuaternionSequence(qs) *= q;
}

const QuaternionSequence operator*
(const Quaternion& q, const QuaternionSequence& qs)
{
    //TODO: mnożenie z lewej strony...
    return QuaternionSequence();
}

const QuaternionSequence operator/
(const QuaternionSequence& qs, const Quaternion& q)
{
    return QuaternionSequence(qs) /= q;
}
