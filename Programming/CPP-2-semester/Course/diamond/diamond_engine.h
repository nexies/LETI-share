//
// Created by nexie on 11.06.2025.
//

#ifndef DIAMOND_ENGINE_H
#define DIAMOND_ENGINE_H

#include "nx_forward_list.h"
#include "marker_string.h"

///
/// Добавить: 1) чтение списка названий компаний из файла
///           2) проверку на корректность наличия данных (?)

namespace diamond {
    struct diamond_mine
    {
        marker_string mine_name { "unknown" };
        nx::forward_list<double> diamonds;
    };
    struct diamond_source
    {
        marker_string company_name { "unknown" };
        double size { 0 };
    };
    struct diamond_source_group
    {
        double lower_bound { 0 };
        double upper_bound { 0 };
        nx::forward_list<diamond_source> sources;
    };

    class diamond_engine {
        nx::forward_list <diamond_mine> _mines {};
        nx::forward_list <diamond_source> _sources {};
        double _max_size {0}, _min_size {0};
        nx::forward_list <diamond_source_group> _groups {};

        void init ();
        void main_loop ();
    public:
        int exec ();

        bool create_source (const marker_string & p_filename, uint32_t p_entries);
        void read_source (const marker_string & p_filename);
        void merge_source ();
        void split_source_into_groups(unsigned p_group_count);
        void sort_sources_groups();
        void print_sources_groups();
        void write_groups(const marker_string & p_filename);
        void check_groups ();
    };

}

#endif //DIAMOND_ENGINE_H
