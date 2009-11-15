#include <iostream>
#include <stdexcept>
#include <map>

#include "./quatsec.h"
#include "./quaternion.h"

#ifdef DEBUGLEVEL
    const int DEBUG_LEVEL = DEBUGLEVEL;
#else
    const int DEBUG_LEVEL = 0;
#endif

typedef QuaternionSequence::ProxyQContainer ProxyQContainer;

typedef QuaternionSequence::size_type size_type;

ProxyQContainer::ProxyQContainer
    (QuaternionSequence::size_type id,
     QuaternionSequence& qs)
  : index(id),
    qs(&qs)
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequece::"
                     "ProxyQContainer::"
                     "ProxyQContainer"
                     "(QuaternionSequence::size_type id);\n"
                  << std::flush;
}

ProxyQContainer::ProxyQContainer(const QuaternionSequence::ProxyQContainer& cnt)
  : index(cnt.index),
    qs(cnt.qs)
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::"
                     "ProxyQContainer"
                     "(QuaternionSequence::ProxyQContainer& cnt);\n"
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
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequece::"
                     "ProxyQContainer::"
                     "~ProxyQContainer();\n"
                  << std::flush;
}

ProxyQContainer* ProxyQContainer::clone() const
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::clone();\n"
                  << std::flush;
    return new ProxyQContainer(index, *qs);
}

Real ProxyQContainer::getA() const
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::getA() const;\n"
                  << std::flush;
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.r();
    else
        return RZERO;
}

Real ProxyQContainer::getB() const
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::getB() const;\n"
                  << std::flush;
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.i();
    else
        return RZERO;
}

Real ProxyQContainer::getC() const
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::getC() const;\n"
                  << std::flush;
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.j();
    else
        return RZERO;
}

Real ProxyQContainer::getD() const
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::getD() const;\n"
                  << std::flush;
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter != qs->seq.end())
        return iter->second.k();
    else
        return RZERO;
}

void ProxyQContainer::setA(Real a)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::setA(Real a);\n"
                     "\tCalled with: " << a << ".\n"
                  << std::flush;
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setA(a);

}

void ProxyQContainer::setB(Real b)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::setB(Real b);\n"
                     "\tCalled with: " << b << ".\n"
                  << std::flush;
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setB(b);

}

void ProxyQContainer::setC(Real c)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::setC(Real c);\n"
                     "\tCalled with: " << c << ".\n"
                  << std::flush;
    std::map<size_type, Quaternion>::iterator iter;
    iter = qs->seq.find(index);
    if(iter == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setC(c);

}

void ProxyQContainer::setD(Real d)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::"
                     "ProxyQContainer::setD(Real d);\n"
                     "\tCalled with: " << d << ".\n"
                  << std::flush;
    if(qs->seq.find(index) == qs->seq.end())
        qs->seq[index] = Quaternion();
    qs->seq[index].val->setD(d);

}

Quaternion& QuaternionSequence::getQuaternion(size_type id)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::getQuaternion"
                     "(size_type id);\n"
                     "\tCalled with: " << id << ".\n"
                  << std::flush;
    if(seq.find(id) == seq.end())
        seq[id] = Quaternion();
    return seq[id];
}

Quaternion& QuaternionSequence::setQuaternion
    (size_type id, const Quaternion& q)
{
    if(DEBUG_LEVEL > 4)
        std::cerr << "(II) "
                     "QuaternionSequence::setQuaternion"
                     "(size_type id, Quaternion q);\n"
                     "\tCalled with (id): " << id << ",\n"
                     "\tCalled with (q):  " << q << ".\n"
                  << std::flush;
    seq[id] = Quaternion(q);
    return seq[id];
}

QuaternionSequence::QuaternionSequence()
  : seq(std::map<size_type, Quaternion>())
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequence::QuaternionSequence();\n"
                  << std::flush;
}

QuaternionSequence::QuaternionSequence
    (const std::map<size_type, Quaternion>& m_seq)
  : seq(m_seq)
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequence::QuaternionSequence"
                     "(const std::map<QuaternionSequence::size_type, "
                     "Quaternion>);\n"
                  << std::flush;
}

QuaternionSequence::QuaternionSequence
    (const std::vector<Quaternion>& v_seq)
  : seq(std::map<size_type, Quaternion>())
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequence::QuaternionSequence"
                     "(const std::vector<Quaternion>& v_seq);\n"
                  << std::flush;
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
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequence::QuaternionSequence"
                     "(const QuaternionSequence& qs);\n"
                  << std::flush;
}

QuaternionSequence::~QuaternionSequence()
{
    if(DEBUG_LEVEL > 3)
        std::cerr << "(II) "
                     "QuaternionSequence::~QuaternionSequence();\n"
                  << std::flush;
}

































