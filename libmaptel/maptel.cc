#include <map>
#include <set>
#include <list>

#include <cassert>

#include <iostream>
#include <sstream>

#include <string>
#include <cstring>

#include "./maptel.h"


#ifdef MAPTEL_DEBUG_LEVEL
  const int DEBUG_LEVEL = MAPTEL_DEBUG_LEVEL;
#else
  const int DEBUG_LEVEL = 0;
#endif


const size_t MAX_STR_LENGTH = 100;


using namespace std;


typedef unsigned long Integer;

typedef string String;


/**
  cerr output description:
  (II) information
  (EE) error
  (WW) warning
*/

/** interface: */

class MapTel {

    private:

        /** identificator; */
        Integer id;

        /** map; */
        map<String, String> tel_transforms;

        /** returns next not used id; */
        static Integer& getNextId();

        /** returns list of deleted and unallocated ids; */
        static list<Integer>& getFreeIdsList();

    protected:

        /** creates empty maptel; */
        MapTel(Integer id);

        /** checks if given number is correct; */
        static bool isCorrect(const String& number);

        /** returns maptels map; */
        static map<Integer, MapTel>& getMap();

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

list<Integer>& MapTel::getFreeIdsList()
{
    /* The static object does not need to be allocated dynamically,
     * because it is not dependent on any other `static` object
     * and any other `static` object depends on `unallocated_ids`. */
    static list<Integer> unallocated_ids = list<Integer>();
    return unallocated_ids;
}

MapTel::MapTel(Integer id) : id(id)
{
    if(DEBUG_LEVEL > 1)
        cerr << "(II) libmaptel -> creating maptel of id = "
            << id << ".\n" << flush;
    tel_transforms = map<String, String>();
}

MapTel::MapTel(const MapTel& copy) : id(copy.getId())
{
    if(DEBUG_LEVEL > 1)
        cerr << "(II) libmaptel -> creating maptel of id = "
            << id << " (copy).\n" << flush;
    tel_transforms = map<String, String>(copy.tel_transforms);
}

bool MapTel::isCorrect(const String& number)
{
    for(String::const_iterator it = number.begin();
        it < number.end();
        it ++)
        if(!(*it >= '0' && *it <= '9')) {
            if(DEBUG_LEVEL > 1)
                cerr << "(EE) libmaptel -> checking if number <"
                    << number << "> is correct... [NO].\n" << flush;
            return false;
        }
    if(DEBUG_LEVEL > 1) {
        cerr << "(II) libmaptel -> checking if number <"
            << number << "> is correct... ";
        if(number.size() > 0)
            cerr << "[YES].\n" << flush;
        else
            cerr << "[NO].\n" << flush;
    }

    return (number.size() > 0);
}

map<Integer, MapTel>& MapTel::getMap()
{
    static map<Integer, MapTel> maptels = map<Integer, MapTel>();
    return maptels;
}

Integer MapTel::shiftNextId()
{
    list<Integer>& unused = getFreeIdsList();
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
    map<Integer,MapTel>::iterator it = getMap().find(id);
    if(DEBUG_LEVEL > 1)
        if(it == getMap().end())
            cerr << "(EE) libmaptel -> maptel of id: " << id
                << " does not exist!\n" << flush;
    return (it != getMap().end());
}

MapTel& MapTel::getMapTel(Integer id)
{
    map<Integer,MapTel>::iterator it = getMap().find(id);
    if(DEBUG_LEVEL > 1)
        if(it == getMap().end())
            cerr << "(EE) libmaptel -> maptel of id: " << id
                << " does not exist!\n" << flush;
    if(DEBUG_LEVEL > 0)
        assert(it != getMap().end());
    return it->second;
}

MapTel& MapTel::createMapTel()
{
    if(DEBUG_LEVEL > 1)
        cerr << "(II) libmaptel -> creating new maptel:\n";
    map<Integer,MapTel>& maptels = getMap();
    Integer id = shiftNextId();
    maptels.insert(pair<Integer, MapTel>(id, MapTel(id)));
    if (DEBUG_LEVEL > 1)
        cerr << "(II) libmaptel -> end creating new maptel.\n" << flush;
    return getMapTel(id);
}

void MapTel::deleteMapTel(Integer id)
{
    bool map_exists = exists(id);
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1) {
            if(!map_exists)
                cerr << "(EE) libmaptel -> trying to delete maptel " << id
                    << " which does not exist.\n" << flush;
            else
                cerr << "(II) libmaptel -> deleting maptel of id = " << id
                    << ".\n" << flush;
        }
        assert(map_exists);
    }
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
    if(DEBUG_LEVEL > 0)
        cerr << "(II) libmaptel -> destroying maptel of id="
            << getId() << flush;
}

void MapTel::insert(const String& source, const String& destination)
{
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1) {
            cerr << "(II) libmaptel -> [id=" << getId() << "]insert(\""
                << source << "\", \"" << destination << "\");\n";
        }
        bool source_correct = isCorrect(source);
        bool destin_correct = isCorrect(destination);
        if(DEBUG_LEVEL > 1) {
            if(source_correct && destin_correct)
                cerr << "(II) libmaptel -> [id=" << getId()
                    << "]insert: given numbers are correct.\n";
            else
                cerr << "(EE) libmaptel -> [id=" << getId()
                    << "]insert: some of given numbers: \""
                    << source << "\", \"" << destination
                    << "\" were incorrect!\n";
            cerr << flush;
        }
        assert(source_correct && destin_correct);
        if(DEBUG_LEVEL > 1) {
            map<String, String>::iterator it = tel_transforms.find(source);
            if(it == tel_transforms.end())
                cerr << "(II) libmaptel -> [id=" << getId()
                    << "]insert: inserting new transform: "
                    << source << " -> " << destination << ".\n";
            else
                cerr << "(II) libmaptel -> [id=" << getId()
                    << "]insert: changing transform to: "
                    << source << " -> " << destination << " (from: "
                    << source << " -> " << it->second << ").\n";
            cerr << flush;
        }
    }
    tel_transforms[source] = destination;
}

void MapTel::erase(const String& source)
{
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            cerr << "(II) libmaptel -> [id=" << getId() << "]erase(\""
                << source << "\");\n";
        bool source_correct = isCorrect(source);
        if(DEBUG_LEVEL > 1) {
            if(source_correct)
                cerr << "(II) libmaptel -> [id=" << getId()
                    << "]erase: given source is correct.\n";
            else
                cerr << "(EE) libmaptel -> [id=" << getId()
                    << "]erase: given source: \"" << source
                    << "\" is not correct!\n";
            cerr << flush;
        }
        assert(source_correct);
        if(DEBUG_LEVEL > 1) {
            map<String, String>::iterator it = tel_transforms.find(source);
            if(it == tel_transforms.end())
                cerr << "(WW) libmaptel -> [id=" << getId()
                    << "]erase: source not found, doing nothing.\n";
            else
                cerr << "(II) libmaptel -> [id=" << getId()
                    << "]erase: source found, erasing transformation: "
                    << source << " -> " << it->second << ".\n";
            cerr << flush;
        }
    }
    tel_transforms.erase(source);
}

String MapTel::transform(const String& source) const
{
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            cerr << "(II) libmaptel -> [id=" << getId() << "]transform(\""
                << source << "\");\n";
        bool source_correct = isCorrect(source);
        if(DEBUG_LEVEL > 1) {
            if(source_correct)
                cerr << "(II) libmaptel -> [id=" << getId()
                    << "]transform: given source is correct.\n";
            else
                cerr << "(EE) libmaptel -> [id=" << getId()
                    << "]transform: given source: \"" << source
                    << "\" is not correct!\n";
            cerr << flush;
        }
        assert(source_correct);
    }

    map<String, String>::const_iterator it = tel_transforms.find(source);

    if(DEBUG_LEVEL > 1) {
        if(it == tel_transforms.end())
            cerr << "(II) libmaptel -> [id=" << getId()
                << "]transform: source not found, returning id transformation: "
                << source << " -> " << source << ".\n";
        else
            cerr << "(II) libmaptel -> [id=" << getId()
                << "]transform: source found, returning transformation: "
                << source << " -> " << it->second << ".\n";
        cerr << flush;
    }

    if(it != tel_transforms.end())
        return it->second;
    return String(source);
}

bool MapTel::isCyclic(const String& source) const
{
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            cerr << "(II) libmaptel -> [id=" << getId() << "]isCyclic(\""
                << source << "\");\n";
        bool source_correct = isCorrect(source);
        if(DEBUG_LEVEL > 1) {
            if(source_correct)
                cerr << "(II) libmaptel -> [id=" << getId()
                    << "]isCyclic: given source is correct.\n";
            else
                cerr << "(EE) libmaptel -> [id=" << getId()
                    << "]isCyclic: given source: \"" << source
                    << "\" is not correct!\n";
            cerr << flush;
        }
        assert(source_correct);
    }
    String current_source = String(source);
    set<String> seen = set<String>();
    set<String>::iterator seen_it;
    map<String,String>::const_iterator maptel_it;
    if(DEBUG_LEVEL > 1)
        cerr << "(II) libmaptel -> isCyclic: checking cycle from source: "
            << source << ";\n";
    while(true) {
        seen_it = seen.find(current_source);
        if(seen_it != seen.end()) {
            if(DEBUG_LEVEL > 1)
                cerr << "(II) libmaptel -> isCyclic: element: "
                    << current_source << " was already seen;\n"
                    << "(II) libmaptel -> isCyclic: cycle found.\n" << flush;
            return true;
        }
        seen.insert(current_source);
        maptel_it = tel_transforms.find(current_source);
        if(maptel_it != tel_transforms.end()) {
            if(DEBUG_LEVEL > 1)
                cerr << "(II) libmaptel -> isCyclic: transform: "
                    << current_source << " -> " << maptel_it->second << ";\n";
            current_source = (*maptel_it).second;
        }
        else {
            if(DEBUG_LEVEL > 1)
                cerr << "(II) libmaptel -> isCyclic: transform: "
                    << current_source << " -> ... (none found);\n"
                    << "(II) libmaptel -> isCyclic: cycle NOT found.\n"
                    << flush;
            return false;
        }
    }
}

String MapTel::transformEx(const String& source) const
{
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            cerr << "(II) libmaptel -> [id=" << getId() << "]transformEx(\""
                << source << "\");\n";
        bool source_correct = isCorrect(source);
        if(DEBUG_LEVEL > 1) {
            if(source_correct)
                cerr << "(II) libmaptel -> [id=" << getId()
                    << "]transformEx: given source is correct.\n";
            else
                cerr << "(EE) libmaptel -> [id=" << getId()
                    << "]transformEx: given source: \"" << source
                    << "\" is not correct!\n";
            cerr << flush;
        }
        assert(source_correct);
    }
    String current_source = String(source);
    set<String> seen = set<String>();
    set<String>::iterator seen_it;
    map<String,String>::const_iterator maptel_it;
    if(DEBUG_LEVEL > 1)
        cerr << "(II) libmaptel -> transformEx: checking path from: "
            << source << ";\n";
    while(true) {
        seen_it = seen.find(current_source);
        if(DEBUG_LEVEL > 0) {
            if(DEBUG_LEVEL > 1)
                if(seen_it != seen.end())
                    cerr << "(EE) libmaptel -> transformEx: element: "
                        << current_source << " was already seen;\n"
                        << "(EE) libmaptel -> transformEx: cycle found!\n"
                        << flush;
            assert(seen_it == seen.end());
        }
        if(seen_it != seen.end())
            break;
        seen.insert(current_source);
        maptel_it = tel_transforms.find(current_source);
        if(maptel_it != tel_transforms.end()) {
            if(DEBUG_LEVEL > 1)
                cerr << "(II) libmaptel -> transformEx: transform: "
                    << current_source << " -> " << maptel_it->second << ";\n";
            current_source = (*maptel_it).second;
        }
        else {
            if(DEBUG_LEVEL > 1)
                cerr << "(II) libmaptel -> transformEx: transform: "
                    << current_source << " -> ... (none found);\n"
                    << "(II) libmaptel -> transformEx: "
                    "final destination found: "
                    << current_source << "\n"
                    << flush;
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
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            if(tel_src == NULL)
                cerr << "(EE) libmaptel -> insert: tel_src is NULL!\n"
                    << flush;
        assert(tel_src != NULL);
        if(DEBUG_LEVEL > 1)
            if(tel_dst == NULL)
                cerr << "(EE) libmapter -> insert: tel_dst is NULL!\n"
                    << flush;
        assert(tel_dst != NULL);
        if(DEBUG_LEVEL > 1)
            if(!MapTel::exists(id))
                cerr << "(EE) libmaptel -> insert: maptel of id = " << id
                    << " does not exist!\n" << flush;
        assert(MapTel::exists(id));
    }
    String src = String(tel_src);
    String dst = String(tel_dst);
    MapTel::getMapTel(id).insert(src, dst);
}

void maptel_erase(unsigned long id, const char *tel_src)
{
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            if(tel_src == NULL)
                cerr << "(EE) libmaptel -> erase: tel_src is NULL!\n"
                    << flush;
        assert(tel_src != NULL);
        if(DEBUG_LEVEL > 1)
            if(!MapTel::exists(id))
                cerr << "(EE) libmaptel -> erase: maptel of id = " << id
                    << " does not exist!\n" << flush;
        assert(MapTel::exists(id));
    }
    String src = String(tel_src);
    MapTel::getMapTel(id).erase(src);
}

void maptel_transform
(unsigned long id, const char *tel_src, char *tel_dst, size_t len)
{
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            if(tel_src == NULL)
                cerr << "(EE) libmaptel -> transform: tel_src is NULL!\n"
                    << flush;
        assert(tel_src != NULL);
        if(DEBUG_LEVEL > 1)
            if(tel_dst == NULL)
                cerr << "(EE) libmaptel -> transform: tel_dst is NULL!\n"
                    << flush;
        if(DEBUG_LEVEL > 1)
            if(len < 1)
                cerr << "(EE) libmaptel -> transform: len must be >= 1!\n"
                    << flush;
        assert(len >= 1);
        if(DEBUG_LEVEL > 1)
            if(!MapTel::exists(id))
                cerr << "(EE) libmaptel -> transform: maptel of id = " << id
                    << " does not exist!\n";
        assert(MapTel::exists(id));
    }
    const String src = String(tel_src);
    const String dst = MapTel::getMapTel(id).transform(src);
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1) {
            if(len < dst.size() + 1)
                cerr << "(EE) libmaptel -> transform: amount of given memory ("
                    << sizeof(char) * len
                    << "B) to small for writing returned dest: "
                    "#\"" << dst << "\\0\" = " << dst.size() + 1 <<
                    " > " << len << ".\n" << flush;
        else
            cerr << "(EE) libmaptel -> transform: amount of given memory ("
                    << sizeof(char) * len << "B) correct: "
                    "#\"" << dst << "\\0\" = " << dst.size() + 1
                    << " <= " << len << ".\n" << flush;
        }
        assert(dst.size() + 1 <= len);
    }
    for(size_t i = 0; i < dst.size(); i ++)
        tel_dst[i] = dst.at(i);
    tel_dst[dst.size()] = '\0';
}

int maptel_is_cyclic(unsigned long id, const char *tel_src)
{
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            if(tel_src == NULL)
                cerr << "(EE) libmaptel -> isCyclic: tel_src is NULL!\n"
                    << flush;
        assert(tel_src != NULL);
        if(DEBUG_LEVEL > 1)
            if(!MapTel::exists(id))
                cerr << "(EE) libmaptel -> isCyclic: maptel of id = " << id
                    << " does not exist!\n";
        assert(MapTel::exists(id));
    }
    const String src = String(tel_src);
    return static_cast<int>(MapTel::getMapTel(id).isCyclic(src));
}

void maptel_transform_ex
(unsigned long id, const char *tel_src, char *tel_dst, size_t len)
{
        if(DEBUG_LEVEL > 1)
            if(tel_src == NULL)
                cerr << "(EE) libmaptel -> transformEx: tel_src is NULL!\n"
                    << flush;
        assert(tel_src != NULL);
        id(DEBUG_LEVEL > 1)
            if(tel_dst == NULL)
                cerr << "(EE) libmaptel -> transformEx: tel_dst is NULL!\n"
                    << flush;
        assert(tel_dst != NULL);
        if(DEBUG_LEVEL > 1)
            if(len < 1)
                cerr << "(EE) libmaptel -> transformEx: len must be >= 1!\n"
                    << flush;
        assert(len >= 1);
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1)
            if(!MapTel::exists(id))
                cerr << "(EE) libmaptel -> transformEx: maptel of id = " << id
                    << " does not exist!\n";
        assert(MapTel::exists(id));
    }
    const String src = String(tel_src);
    const String dst = MapTel::getMapTel(id).transformEx(src);
    if(DEBUG_LEVEL > 0) {
        if(DEBUG_LEVEL > 1) {
            if(len < dst.size() + 1)
                cerr << "(EE) libmaptel -> transformEx: amount of "
                    "given memory (" << sizeof(char) * len << "B) to small "
                    "for writing returned dest: "
                    "#\"" << dst << "\\0\" = " << dst.size() + 1
                    << " > " << len << ".\n" << flush;
        else
            cerr << "(EE) libmaptel -> transformEx: amount of given memory ("
                << sizeof(char) * len << "B) correct: "
                << "#\"" << dst << "\\0\" = " << dst.size() + 1 << " <= " << len
                << ".\n" << flush;
        }
        assert(dst.size() + 1 <= len);
    }
    for(size_t i = 0; i < dst.size(); i ++)
        tel_dst[i] = dst.at(i);
    tel_dst[dst.size()] = '\0';
}


int main()
{
    Integer map1 = maptel_create();
    Integer map2 = maptel_create();
    maptel_delete(map1);
    Integer map3 =  maptel_create();
    map1 = maptel_create();
    char* tel_dst = new char[100]();
    maptel_insert(map1, "1234", "5678");
    maptel_insert(map1, "1234", "5679");
    maptel_transform(map2, "1234", tel_dst, 100);
    maptel_insert(map1, "5679", "7777");
    maptel_insert(map1, "7777", "1234");
    assert(maptel_is_cyclic(map1, "7777"));
    maptel_insert(map1, "5679", "8888");
    assert(!maptel_is_cyclic(map1, "7777"));
    maptel_erase(map1, "5679");
    maptel_insert(map3, "1234", "2345");
    maptel_insert(map3, "2345", "2345");
    assert(maptel_is_cyclic(map3, "1234"));
    maptel_erase(map3, "2345");
    assert(!maptel_is_cyclic(map3, "1234"));
    return 0;
}
