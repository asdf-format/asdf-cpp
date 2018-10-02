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

#include <asdf-cpp/asdf.hpp>
#include <asdf-cpp/block.hpp>
#include <asdf-cpp/compression.hpp>
#include <asdf-cpp/tags/ndarray.hpp>

#define ASDF_HEADER             "#ASDF"
#define ASDF_STANDARD_HEADER    "#ASDF_STANDARD"
#define YAML_START_MARKER       "---"
#define YAML_END_MARKER         "..."


static bool parse_header(std::istream &stream)
{
    std::string line;
    std::getline(stream, line);

    if (line.compare(0, strlen(ASDF_HEADER), ASDF_HEADER))
    {
        return false;
    }

    std::getline(stream, line);
    if (line.compare(0, strlen(ASDF_STANDARD_HEADER), ASDF_STANDARD_HEADER))
    {
        return false;
    }

    return true;
}

static std::streampos
find_yaml_end(std::stringstream &yaml, std::istream &stream)
{
    std::string line;

    while(std::getline(stream, line))
    {
        yaml << line << std::endl;
        if (line.compare(YAML_END_MARKER) == 0)
        {
            break;
        }
    }

    return stream.tellg();
}

namespace Asdf {

AsdfFile::AsdfFile()
{
    asdf_tree = Node();
}

AsdfFile::AsdfFile(std::string filename)
{
    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        std::string msg("Error opening " + filename + ": ");
        throw std::runtime_error(msg + strerror(errno));
    }

    if (!parse_header(ifs))
    {
        throw std::runtime_error("Invalid ASDF header");
    }

    std::stringstream yaml_data;
    end_index = find_yaml_end(yaml_data, ifs);

    /* Reset stream to the beginning of the file */
    /* TODO: this should probably be a close */
    ifs.seekg(0);

    setup_memmap(filename);
    find_blocks();

    asdf_tree = YAML::Load(yaml_data);
}

AsdfFile::AsdfFile(std::stringstream &stream)
{
    if (!parse_header(stream))
    {
        throw std::runtime_error("Invalid ASDF header");
    }

    std::stringstream yaml_data;
    end_index = find_yaml_end(yaml_data, stream);

    /* Reset stream to the beginning of the file */
    /* TODO: this should probably be a close */
    stream.seekg(0);

    copy_stream(stream);
    find_blocks();

    asdf_tree = YAML::Load(yaml_data);
}

AsdfFile::~AsdfFile()
{
    if (memmapped)
    {
        munmap(data, data_size);
        close(fd);
    }
    else
    {
        /* TODO: try using shared pointer instead */
        free(data);
    }
}

void AsdfFile::setup_memmap(std::string filename)
{
    struct stat sb;

    fd = open(filename.c_str(), O_RDONLY);

    fstat(fd, &sb);
    data_size = sb.st_size;

    data = (uint8_t *) mmap(NULL, data_size, PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        close(fd);
        std::string msg("Error memory mapping file: ");
        throw std::runtime_error(msg + strerror(errno));
    }

    memmapped = true;
}

void AsdfFile::copy_stream(std::iostream &stream)
{
    stream.seekp(0, std::ios::end);
    data_size = stream.tellp();

    std::basic_streambuf<char> *sbuf = stream.rdbuf();
    data = (uint8_t *) malloc(data_size);
    if (data == nullptr)
    {
        std::string msg("Error creating buffer for file data: ");
        throw std::runtime_error(msg + strerror(errno));
    }

    sbuf->sgetn((char *) data, data_size);
}

void AsdfFile::find_blocks()
{
    uint8_t *current = data + end_index;

    while ((current + sizeof(block_header_t)) < (data + data_size))
    {
        block_header_t *bh = (block_header_t *)(current);

        /* Indicates we found the block index */
        if (memcmp(bh->magic, "#ASD", sizeof(bh->magic)) == 0)
        {
            break;
        }
        else if (memcmp(bh->magic, asdf_block_magic, sizeof(bh->magic)) != 0)
        {
            throw std::runtime_error("Invalid block header");
        }

        blocks.push_back(current);
        current += bh->get_allocated_size() + bh->total_header_size();
    }
}

Node AsdfFile::get_tree()
{
    return asdf_tree;
}

Node AsdfFile::operator[] (std::string key)
{
    return asdf_tree[key];
}

void AsdfFile::write_blocks(std::ostream &ostream) const
{
    block_manager.write_blocks(ostream);

    /* TODO: write block index here as well */
}

std::ostream& operator<<(std::ostream& stream, const AsdfFile &af)
{
    stream << ASDF_HEADER << " " << ASDF_FILE_FORMAT_VERSION << std::endl;
    stream << ASDF_STANDARD_HEADER << " " << ASDF_STANDARD_VERSION << std::endl;
    stream << "%YAML 1.1" << std::endl;
    stream << "%TAG ! tag:stsci.edu:asdf/" << std::endl;
    /* TODO: there may be a more general way to handle the top-level object */
    stream << YAML_START_MARKER << " " << "!core/asdf-1.1.0" << std::endl;

    stream << af.asdf_tree;

    stream << std::endl << YAML_END_MARKER << std::endl;

    af.write_blocks(stream);

    return stream;
}

} /* namespace Asdf */
