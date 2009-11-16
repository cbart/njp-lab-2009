#include <map>
#include <set>
#include <list>

#include <cassert>

#include <iostream>
#include <sstream>

#include <string>
#include <cstring>

#include "./maptel.h"

typedef unsigned long Integer;

typedef std::string String;

#ifdef MAPTEL_DEBUG_LEVEL
    const Integer DEBUG_LEVEL = MAPTEL_DEBUG_LEVEL;
    #if MAPTEL_DEBUG_LEVEL == 0
        #define NDEBUG
    #endif
#else
    const Integer DEBUG_LEVEL = 0;
    #define NDEBUG
#endif


const size_t MAX_STR_LENGTH = 100;


/**
  std::cerr output description:
  (II) information
  (EE) error
  (WW) warning
*/

/** Error and warnings handling class. */
class DebugStream {

    private:

        const Integer MIN_DEBUG_LEVEL;

    public:

        static const String LIB_NAME;

        DebugStream(Integer min_debug);

        template<typename T>
        friend DebugStream& operator<<(DebugStream& ds, const T& message);

        friend DebugStream& operator<<
            (DebugStream& ds, std::ostream& (*message_fun)(std::ostream&));
};

/** Name of current library displayed with every diagnostic info. */
const String DebugStream::LIB_NAME = " libmaptel -> ";

/** Error and warnings handling implementation. */

DebugStream::DebugStream(Integer min_debug = 0)
    : MIN_DEBUG_LEVEL(min_debug)
{
}

template<typename T>
DebugStream& operator<<(DebugStream& ds, const T& message)
{
    if(DEBUG_LEVEL >= ds.MIN_DEBUG_LEVEL)
        std::cerr << message;
    return ds;
}

DebugStream& operator<<
    (DebugStream& ds, std::ostream& (*message_fun)(std::ostream&))
{
    if(DEBUG_LEVEL >= ds.MIN_DEBUG_LEVEL)
        std::cerr << message_fun;
    return ds;
}

DebugStream& debug_info()
{
    static DebugStream info_stream = DebugStream(2);
        if(DEBUG_LEVEL >= 2)
            std::cerr << "(II)" << DebugStream::LIB_NAME;
    return info_stream;
}

DebugStream& debug_warn()
{
    static DebugStream warning_stream = DebugStream(2);
        if(DEBUG_LEVEL >= 2)
            std::cerr << "(WW)" << DebugStream::LIB_NAME;
    return warning_stream;
}

DebugStream& debug_err()
{
    static DebugStream error_stream = DebugStream(2);
        if(DEBUG_LEVEL >= 2)
            std::cerr << "(EE)" << DebugStream::LIB_NAME;
    return error_stream;
}

class MapTel {

    private:

        /** identificator; */
        Integer id;

        /** map; */
        std::map<String, String> tel_transforms;

        /** returns next not used id; */
        static Integer& getNextId();

        /** returns list of deleted and unallocated ids; */
        static std::list<Integer>& getFreeIdsList();

    protected:

        /** creates empty maptel; */
        MapTel(Integer id);

        /** checks if given number is correct; */
        static bool isCorrect(const String& number);

        /** returns maptels map; */
        static std::map<Integer, MapTel>& getMap();

        /** returns next free id; */
        static Integer shiftNextId();

    public:

        /** copying constructor; */
        MapTel(const MapTel& copy);

        /** true if maptel of given id exists; */
        static bool exists(Integer id);

        /** returns maptel of given id; */
        static MapTel& getMapTel(Integer id);

        /** creates a new maptel; */
        static MapTel& createMapTel();

        /** deletes maptel of given id; */
        static void deleteMapTel(Integer id);

        /** returns maptel's id; */
        Integer getId() const;

        /** inserts given transformation into maptel; */
        void insert(const String& source, const String& destination);

        /** erases transformation from given source (not recursive); */
        void erase(const String& source);

        /** gives transformation from given source (not recursive); */
        String transform(const String& source) const;

        /** true if given source is in a cycle transformation; */
        bool isCyclic(const String& source) const;

        /** gives transformation from given source (recursive); */
        String transformEx(const String& source) const;

        /** returns stringstream with diagnostic information about maptel; */
        //const stringstream& diag();

        /** the destructor; */
        virtual ~MapTel();

};

/** implementation: */


Integer& MapTel::getNextId()
{
    /* `Integer` is always a base type, and because of that
     * we don't need to allocate it dynamically here. */
    static Integer next_id = 0;
    return next_id;
}

std::list<Integer>& MapTel::getFreeIdsList()
{
    /* The static object does not need to be allocated dynamically
     * (aka via `new`), because it is not dependent on any other
     * `static` object and any other `static` object depends
     * on `unallocated_ids`. */
    static std::list<Integer> unallocated_ids = std::list<Integer>();
    return unallocated_ids;
}

MapTel::MapTel(Integer id) : id(id)
{
    debug_info() << "creating maptel of id = " << id << ".\n"
        << std::flush;
    tel_transforms = std::map<String, String>();
}

MapTel::MapTel(const MapTel& copy) : id(copy.getId())
{
    debug_info() << "creating maptel of id = " << id << " (copy).\n"
        << std::flush;
    tel_transforms = std::map<String, String>(copy.tel_transforms);
}

bool MapTel::isCorrect(const String& number)
{
    for(String::const_iterator it = number.begin();
        it < number.end();
        it ++)
        if(!(*it >= '0' && *it <= '9')) {
            debug_err() << "checking if number <"
                << number << "> is correct... [NO].\n" << std::flush;
            return false;
        }
    debug_info() << "checking if number <" << number << "> is correct... \n"
        << std::flush;
    if(number.size() > 0)
        debug_info() << "[YES].\n" << std::flush;
    else
        debug_info() << "[NO].\n" << std::flush;
    return (number.size() > 0);
}

std::map<Integer, MapTel>& MapTel::getMap()
{
    /* The static object does not need to be allocated dynamically
     * (aka via `new`), because it is not dependent on any other
     * `static` object and any other `static` object depends
     * on `maptels`. */
    static std::map<Integer, MapTel> maptels = std::map<Integer, MapTel>();
    return maptels;
}

Integer MapTel::shiftNextId()
{
    std::list<Integer>& unused = getFreeIdsList();
    Integer next;
    if(unused.empty())
        next = getNextId() ++;
    else {
        next = unused.front();
        unused.pop_front();
    }
    return next;
}

bool MapTel::exists(Integer id)
{
    std::map<Integer,MapTel>::iterator it = getMap().find(id);
    if(it == getMap().end())
        debug_err() << "maptel of id: " << id << " does not exist!\n"
            << std::flush;
    return (it != getMap().end());
}

MapTel& MapTel::getMapTel(Integer id)
{
    std::map<Integer,MapTel>::iterator it = getMap().find(id);
    if(it == getMap().end())
        debug_err() << "maptel of id: " << id << " does not exist!\n"
            << std::flush;
    assert(it != getMap().end());
    return it->second;
}

MapTel& MapTel::createMapTel()
{
    debug_info() << "create: creating new maptel:\n";
    std::map<Integer, MapTel>& maptels = getMap();
    Integer id = shiftNextId();
    maptels.insert(std::pair<Integer, MapTel>(id, MapTel(id)));
    debug_info() << "create: end creating new maptel.\n" << std::flush;
    return getMapTel(id);
}

void MapTel::deleteMapTel(Integer id)
{
    bool map_exists = exists(id);
    if(!map_exists)
        debug_err() << "erase: trying to delete maptel " << id
            << " which does not exist.\n" << std::flush;
    else
        debug_info() << "erase: deleting maptel of id = " << id << ".\n"
            << std::flush;
    assert(map_exists);
    if(map_exists) {
        getMap().erase(id);
        getFreeIdsList().push_back(id);
    }
}

Integer MapTel::getId() const
{
    return this->id;
}

MapTel::~MapTel() {
    debug_info() << "erase: destroying maptel of id = " << getId()
        << ".\n" << std::flush;
}

void MapTel::insert(const String& source, const String& destination)
{
    debug_info() << "[id=" << getId() << "]insert:\n"
        << std::flush;
    assert(isCorrect(source));
    assert(isCorrect(destination));
    std::map<String, String>::iterator it = tel_transforms.find(source);
    if(it == tel_transforms.end())
        debug_info() << "inserting new transform: "
            << source << " -> " << destination << ".\n";
    else
        debug_info() << "changing transform "
            << "to: " << source << " -> " << destination << " ("
            << "from: " << source << " -> " << it->second << ").\n"
            << std::flush;
    tel_transforms[source] = destination;
}

void MapTel::erase(const String& source)
{
    assert(isCorrect(source));
    std::map<String, String>::iterator it = tel_transforms.find(source);
    if(it == tel_transforms.end())
        debug_warn() << "erase: source not found, doing nothing.\n"
            << std::flush;
    else
        debug_info() << "erase: source found, erasing transformation: "
            << source << " -> " << it->second << ".\n" << std::flush;
    tel_transforms.erase(source);
}

String MapTel::transform(const String& source) const
{
    assert(isCorrect(source));
    std::map<String, String>::const_iterator it = tel_transforms.find(source);
    if(it == tel_transforms.end())
        debug_info() << "transform: source not found, returning "
            << "`ident` transformation: " << source << " -> " << source << ".\n"
            << std::flush;
    else
        debug_info() << "transform: source found, returning transformation: "
            << source << " -> " << it->second << ".\n" << std::flush;
    if(it != tel_transforms.end())
        return it->second;
    return String(source);
}

bool MapTel::isCyclic(const String& source) const
{
    assert(isCorrect(source));
    String current_source = String(source);
    std::set<String> seen = std::set<String>();
    std::set<String>::iterator seen_it;
    std::map<String,String>::const_iterator maptel_it;
    debug_info() << "isCyclic: checking cycle from source: " << source << ";\n"
        << std::flush;
    while(true) {
        seen_it = seen.find(current_source);
        if(seen_it != seen.end()) {
            debug_info() << "isCyclic: element " << current_source
                << " was already seen;\n" << std::flush;
            debug_info() << "isCyclic: cycle found;\n" << std::flush;
            return true;
        }
        seen.insert(current_source);
        maptel_it = tel_transforms.find(current_source);
        if(maptel_it != tel_transforms.end()) {
            debug_info() << "isCyclic: transform: " << current_source << " -> "
                << maptel_it->second << ";\n" << std::flush;
            current_source = (*maptel_it).second;
        }
        else {
            debug_info() << "isCyclic: transform: " << current_source
                << " -> ... (none found);\n" << std::flush;
            debug_info() << "isCyclic: cycle NOT found;\n" << std::flush;
            return false;
        }
    }
}

String MapTel::transformEx(const String& source) const
{
    assert(isCorrect(source));
    String current_source = String(source);
    std::set<String> seen = std::set<String>();
    std::set<String>::iterator seen_it;
    std::map<String,String>::const_iterator maptel_it;
    debug_info() << "transformEx: checking path from: " << source << ";\n"
        << std::flush;
    while(true) {
        seen_it = seen.find(current_source);
        if(seen_it != seen.end()) {
            debug_err() << "transformEx: element: "
                << current_source << " was already seen;\n" << std::flush;
            debug_err() << "transformEx: cycle found!\n" << std::flush;
        }
        assert(seen_it == seen.end());
        if(seen_it != seen.end())
            break;
        seen.insert(current_source);
        maptel_it = tel_transforms.find(current_source);
        if(maptel_it != tel_transforms.end()) {
            debug_info() << "transformEx: transform: "
                << current_source << " -> " << maptel_it->second << ";\n"
                << std::flush;
            current_source = (*maptel_it).second;
        }
        else {
            debug_info() << "transformEx: transform: "
                << current_source << " -> ... (none found);\n" << std::flush;
            debug_info() << "transformEx: "
                << "final destination found: "
                << current_source << "\n" << std::flush;
            break;
        }
    }
    return String(current_source);
}

unsigned long maptel_create()
{
    return MapTel::createMapTel().getId();
}

void maptel_delete(unsigned long id)
{
    MapTel::deleteMapTel(id);
}

void maptel_insert
(unsigned long id, const char *tel_src, const char *tel_dst)
{
    debug_info() << "[id=" << id << "]insert:\n"
        << std::flush;
    /* Check if pointers are correct. */
    if(tel_src == NULL)
        debug_err() << "insert: tel_src is NULL!\n"
            << std::flush;
    if(tel_dst == NULL)
        debug_err() << "insert: tel_dst is NULL!\n"
            << std::flush;
    assert(tel_src != NULL);
    assert(tel_dst != NULL);
    /* Check if map exists. */
    if(!MapTel::exists(id))
        debug_err() << "insert: maptel of id = "
                    << id << " does not exist!\n"
                    << std::flush;
    assert(MapTel::exists(id));
    if((tel_src != NULL) && (tel_dst != NULL) && (MapTel::exists(id))) {
        String src = String(tel_src);
        String dst = String(tel_dst);
        MapTel::getMapTel(id).insert(src, dst);
    }
}

void maptel_erase(unsigned long id, const char *tel_src)
{
    debug_info() << "[id=" << id << "]erase:\n"
        << std::flush;
    if(tel_src == NULL)
        debug_err() << "erase: tel_src is NULL!\n"
            << std::flush;
    assert(tel_src != NULL);
    if(!MapTel::exists(id))
        debug_err() << "erase: maptel of id = "
            << id << " does not exist!\n" << std::flush;
    assert(MapTel::exists(id));
    if(tel_src != NULL) {
        String src = String(tel_src);
        MapTel::getMapTel(id).erase(src);
    }
}

void maptel_transform
(unsigned long id, const char *tel_src, char *tel_dst, size_t len)
{
    debug_info() << "[id=" << id << "]transform:\n" << std::flush;
    /* Check pointers. */
    if(tel_src == NULL)
        debug_err() << "transform: tel_src is NULL!\n" << std::flush;
    if(tel_dst == NULL)
        debug_err() << "transform: tel_dst is NULL!\n" << std::flush;
    if(len < 1)
        debug_err() << "transform: len must be >= 1!\n" << std::flush;
    if(!MapTel::exists(id))
        debug_err() << "transform: maptel of id = " << id
            << " does not exist!\n" << std::flush;
    assert(tel_src != NULL);
    assert(tel_dst != NULL);
    assert(len >= 1);
    assert(MapTel::exists(id));
    if(tel_src != NULL && tel_dst != NULL && len > 1 && MapTel::exists(id)) {
        const String src = String(tel_src);
        const String dst = MapTel::getMapTel(id).transform(src);
        if(len < dst.size() + 1)
            debug_err() << "transform: amount "
                << "of given memory (" << sizeof(char) * len
                << "B) is to small for writing returned dest: "
                << "#\"" << dst << "\\0\" = " << dst.size() + 1
                << " > " << len << ".\n" << std::flush;
        else
            debug_info() << "transform: amount "
                << "of given memory (" << sizeof(char) * len << "B) correct: "
                << "#\"" << dst << "\\0\" = " << dst.size() + 1
                << " <= " << len << ".\n" << std::flush;
        assert(dst.size() + 1 <= len);
        /* Writing. */
        for(size_t i = 0; i < dst.size(); i ++)
            tel_dst[i] = dst.at(i);
        tel_dst[dst.size()] = '\0';
    }
}

int maptel_is_cyclic(unsigned long id, const char *tel_src)
{
    debug_info() << "[id=" << id << "]isCyclic:\n" << std::flush;
    /* Check pointer. */
    if(tel_src == NULL)
        debug_err() << "isCyclic: tel_src is NULL!\n" << std::flush;
    assert(tel_src != NULL);
    if(!MapTel::exists(id))
        debug_err() << "isCyclic: maptel of id = "
            << id << " does not exist!\n";
    assert(MapTel::exists(id));
    if(tel_src != NULL && MapTel::exists(id)) {
        const String src = String(tel_src);
        return static_cast<int>(MapTel::getMapTel(id).isCyclic(src));
    }
    return -1;
}

void maptel_transform_ex
(unsigned long id, const char *tel_src, char *tel_dst, size_t len)
{
    debug_info() << "[id=" << id << "]transform:\n" << std::flush;
    /* Check pointers. */
    if(tel_src == NULL)
        debug_err() << "transform: tel_src is NULL!\n" << std::flush;
    if(tel_dst == NULL)
        debug_err() << "transform: tel_dst is NULL!\n" << std::flush;
    if(len < 1)
        debug_err() << "transform: len must be >= 1!\n" << std::flush;
    if(!MapTel::exists(id))
        debug_err() << "transform: maptel of id = " << id
            << " does not exist!\n" << std::flush;
    assert(tel_src != NULL);
    assert(tel_dst != NULL);
    assert(len >= 1);
    assert(MapTel::exists(id));
    if(tel_src != NULL && tel_dst != NULL && MapTel::exists(id)) {
        const String src = String(tel_src);
        const String dst = MapTel::getMapTel(id).transformEx(src);
        if(len < dst.size() + 1)
            debug_err() << "transformEx: amount of "
                << "given memory (" << sizeof(char) * len << "B) to small "
                << "for writing returned dest: "
                << "#\"" << dst << "\\0\" = " << dst.size() + 1
                << " > " << len << ".\n" << std::flush;
        else {
            debug_info() << "transformEx: amount "
                << "of given memory (" << sizeof(char) * len << "B) correct: "
                << "#\"" << dst << "\\0\" = " << dst.size() + 1 << " <= "
                << len << ".\n" << std::flush;
            for(size_t i = 0; i < dst.size(); i ++)
                tel_dst[i] = dst.at(i);
            tel_dst[dst.size()] = '\0';
        }
        assert(dst.size() + 1 <= len);
    }
}

