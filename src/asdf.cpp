#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <cstdio>
#include <cstdint>
#include <cstring>
#ifdef __APPLE__
#include <mach/error.h>
#else
#include <error.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

#include <yaml-cpp/yaml.h>

#include <asdf.hpp>
#include <node.hpp>
#include <private/parser.hpp>
#include <private/block.hpp>

#define ASDF_HEADER             "#ASDF"
#define ASDF_STANDARD_HEADER    "#ASDF_STANDARD"
#define YAML_END_MARKER         "..."


static bool parse_header(std::ifstream &ifs)
{
    std::string line;
    std::getline(ifs, line);

    if (line.compare(0, strlen(ASDF_HEADER), ASDF_HEADER))
    {
        return false;
    }

    std::getline(ifs, line);
    if (line.compare(0, strlen(ASDF_STANDARD_HEADER), ASDF_STANDARD_HEADER))
    {
        return false;
    }

    return true;
}

static std::streampos
find_yaml_end(std::stringstream &yaml, std::ifstream &ifs)
{
    std::string line;

    while(std::getline(ifs, line))
    {
        yaml << line << std::endl;
        if (line.compare(YAML_END_MARKER) == 0)
        {
            break;
        }
    }

    return ifs.tellg();
}

namespace Asdf {

AsdfFile::AsdfFile(std::string filename)
{
    this->filename = filename;

    ifs.open(this->filename);
    if (ifs.fail())
    {
        std::string msg("Error opening " + filename + ": ");
        throw std::runtime_error(msg + strerror(errno));
    }

    if (!parse_header(ifs))
    {
        throw std::runtime_error("Invalid ASDF header");
    }

    end_index = find_yaml_end(yaml_data, ifs);
    std::cout << "end index=" << end_index << std::endl;

    /* Reset stream to the beginning of the file */
    /* TODO: this should probably be a close */
    ifs.seekg(0);

    setup_memmap();
    find_blocks();

    asdf_tree = Load(yaml_data, this);
}

AsdfFile::~AsdfFile()
{
    /* TODO: the mmap may not have been created in all cases */
    munmap(memmap, file_size);
    close(fd);
}

void AsdfFile::setup_memmap()
{
    struct stat sb;

    fd = open(filename.c_str(), O_RDONLY);

    fstat(fd, &sb);
    file_size = sb.st_size;

    memmap = (uint8_t *) mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
    if (memmap == MAP_FAILED)
    {
        close(fd);
        std::string msg("Error memory mapping file: ");
        throw std::runtime_error(msg + strerror(errno));
    }
}

void AsdfFile::find_blocks()
{
    block_header_t *bh = (block_header_t *)(memmap + end_index);
    if (memcmp(bh->magic, asdf_block_magic, sizeof(bh->magic)))
    {
        throw std::runtime_error("Invalid block header");
    }

    blocks.push_back(memmap + end_index + sizeof(block_header_t));
}

std::string AsdfFile::get_filename()
{
    return filename;
}

Node AsdfFile::get_tree()
{
    return asdf_tree;
}

Node AsdfFile::operator[] (std::string key)
{
    return asdf_tree[key];
}

void * AsdfFile::get_block(int source)
{
    return blocks[source];
}

} /* namespace Asdf */
