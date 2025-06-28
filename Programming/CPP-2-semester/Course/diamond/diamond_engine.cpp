//
// Created by nexie on 11.06.2025.
//

#include "diamond_engine.h"
#include <fstream>
#include <iomanip>
#include "logger.h"

namespace diamond {
    const nx::forward_list<marker_string> diamond_companies {
    "Crystal Edge",
    "Diamond Stream",
    "Shining Point",
    "Bright Core",
    "Pure Gems",
    "RoughSpark Inc",
    "Glint Mines",
    "Stone Nest",
    "Glitter Depth",
    "Facet Group",
    "Radiant Drill",
    "Northern Vein",
    "Hidden Jewel",
    "White Glare",
    "Raw Cutters",
    "Gleam Mining",
    "Gem Ridge",
    "Hard Light",
    "Diamond Pulse",
    "Clarity Diggers",
    "Stoneburst Ltd",
    "Lustre Lab",
    "Gemwork Co",
    "Facet Brothers",
    "TrueCarbon Ltd",
    "Crystalroot Inc",
    "Bright Hollow",
    "Drillstream",
    "Roughstone Co",
    "Deep Spark",
    "Blunt Crystal",
    "ShineValley",
    "Minebeam",
    "Prism Pickers",
    "Rocklight Ltd",
    "Diamond Creek",
    "Brightstone Group",
    "Glint & Rock",
    "Raw Glory",
    "Gem Splitters",
    "Veinflow Corp",
    "Lumen Core",
    "Cold Spark",
    "Crystal Mill",
    "White Edge Co",
    "True Gemline",
    "Stonecore Inc",
    "Brilliance Basin",
    "Deep Carbon",
    "ShinyWorks",
    "FacetField",
    "Carbon Spark",
    "Cutter Ridge",
    "Diamond Drop",
    "Bright Quarry",
    "Gleam Zone",
    "RoughLight Inc",
    "Core Radiance",
    "Gemfront Co",
    "Stone Flash",
    "Sharp Diamond",
    "Clarity Forge",
    "Miners Bright",
    "Crystal Works",
    "Veinlight Inc",
    "True Stone",
    "Hardstone Gems",
    "Depthshine Ltd",
    "Gem Vault",
    "Luster Flow",
    "Hardcore Gems",
    "White Carbon",
    "Deepcore Ltd",
    "Quartzlight",
    "Diamond Drift",
    "Gem Vein",
    "Rock Prism",
    "Facet Blaze",
    "Roughshine Co",
    "Brightforge",
    "Miner's Prism",
    "Shineplate",
    "Stonecut Co",
    "Hard Edge Ltd",
    "Luminous Depths",
    "Drillforge Inc",
    "Gemtrace Ltd",
    "Brightseed Mining",
    "Lustercut",
    "Polish Rock",
    "Crystal Prospect",
    "Hardshine Works",
    "Clarity Mine",
    "Deep Hollow Gems",
    "Rockbeam Ltd",
    "Rawstone Group",
    "Stoneveil Co",
    "Sparklight Inc",
    "Facet Horizon",
    "Bright Path Gems",
    "Deep Sparkle",
    "Shine Hill Mining",
    "TrueGlint"
};

    marker_string get_random_distributor ()
    {
        auto index = rand() % diamond_companies.size();
        return diamond_companies.at(index);
    }

    double get_random_diamond_size (double p_lower, double p_upper)
    {
        unsigned int unsign_rand = (unsigned int)(rand());
        double zero_to_one = (unsign_rand * 1.0) / RAND_MAX;
        return zero_to_one * (p_upper - p_lower) + p_lower;
    }

    bool get_yes_or_with_deault (std::basic_istream<char> & stream, bool default_yes = true)
    {
        std::string answer;
        stream >> answer;
        if (answer.empty())
            return default_yes;
        return answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes";
    }

    void diamond_engine::init()
    {
        srand((unsigned)(time(nullptr)));
        auto & logger = nx::logger::instance();
        logger.set_log_file("diamond.log");

        log_info("\n\nDiamonds - startup initialisation");

        marker_string _credits =
            "======================================================\n"
            "Курсовая работа по дисциплине \"Программирование\"\n"
            "Автор: Иванов Григорий Денисович\n"
            "Группа: 4335 (подгруппа 1)\n"
            "======================================================\n";

        std::cout << _credits << std::endl;
        std::cout << "--- Сортировщик бриллиантов ---\n" << std::endl;

        std::cout << "Суть задания: " << std::endl;
        std::cout << "\tНебогатая фирма по производству бриллиантов получает алмазы с нескольких рудников,\n"
                     "\tс каждого рудника свой контейнер с разным количеством алмазов. \n"
                     "\tТребуется разделить поступившие алмазы по их весу (размеру) на N групп, \n"
                     "\tсохранив информацию о месте их добычи, при этом рассортировав их в пределах каждой группы\n"
        << std::endl;

        std::cout << "Входные данные: "  << std::endl;
        std::cout << "\tN - количество групп отсортированных бриллиантов (по умолчанию: 10)" << std::endl;
        std::cout << "\tinput_file - файл с входными данными (по умолчанию: 'diamonds.in')" << std::endl;
        std::cout << "\toutput_file - файл с выходными данными (по умолчанию: 'diamonds.out')" << std::endl;
    }

    void diamond_engine::main_loop()
    {
        log_info("Entering main loop");
        marker_string input_file = "diamonds.in";
        marker_string output_file = "diamonds.out";
        int N = 10;
        bool running = true;

        auto print_menu = [&] () {
            log_info("printing menu for the user");
            std::cout << "\nВыберите действие: \n"
                         "\t1. Запустить процесс сортировки\n"
                         "\t2. Указать файл входных данных (текущее значение: '" << input_file << "')\n"
                         "\t3. Указать файл выходных данных (текущее значение: '" << output_file << "')\n"
                         "\t4. Указать количество групп сортировки N (текущее значение: " << N << ")\n"
                         "\t5. Сгенерировать файл со случайными входными данными\n"
                         "\t0. Звершить программу"
            << std::endl;
        };
        auto generate_new_input_file = [this] (const marker_string & p_filename) -> bool {
            log_info(nx::format("generating new input file '%s'", p_filename.c_str()));
            if (std::filesystem::exists(p_filename.c_str())) {
                std::cout << "Файл с указанным именем уже существует. Вы хотите переписать его? [y/N]:";
                bool yes = get_yes_or_with_deault(std::cin, false);
                if (!yes)
                    return false;
                log_warn("an existing file will be overwritten with new data");
            }
            uint64_t entries;
            std::cout << "Введите количество записей в файле: ";
            std::cin >> entries;
            this->create_source(p_filename, entries);
            // this->create_random_source(entries);
            // this->write_source(p_filename);
            this->_sources.clear();
            std::cout << "Данные записаны в файл " << p_filename << std::endl;
            return true;
        };
        auto check_input_file_exists = [generate_new_input_file] (const marker_string & p_filename) -> bool {
            if (std::filesystem::exists(p_filename.c_str()))
                return true;
            log_info (nx::format("file '%s' does not exist. will ask the user to create one", p_filename.c_str()));
            std::cout << "Файл '" << p_filename << "' не найден. Вы хотите сгенерировать новый? [Y/n]:";
            bool yes = get_yes_or_with_deault(std::cin, true);
            if (!yes)
                return false;
            generate_new_input_file(p_filename);
            return yes;
        };

        while (running)
        {
            log_info ("waiting for user input");
            int choise = 0;
            print_menu();
            std::cout << "> ";
            std::cin >> choise;
            switch (choise) {
                case 0: {
                    log_info("exising main loop");
                    running = false;
                    break;
                }
                case 2: {
                    log_info("set input file name option chosen");
                    std::cout << "Введите новое значение: ";
                    std::string std_input_file;
                    std::cin >> std_input_file;
                    input_file = marker_string(std_input_file);
                    log_info (nx::format("input file name is set to '%s'", input_file.c_str()));
                    break;
                }
                case 3: {
                    log_info("set output file name option chosen");
                    std::cout << "Введите новое значение: ";
                    std::string std_output_file;
                    std::cin >> std_output_file;
                    output_file = marker_string(std_output_file);
                    break;
                    log_info(nx::format("output file name is set to '%s'", output_file.c_str()));
                }
                case 4: {
                    log_info(marker_string("set group count option chosen"));
                    do {
                        std::cout << "Введите новое значение (от 1 до 1000): ";
                        std::cin >> N;
                        if (N <= 0 || N > 1000)
                        {
                            log_warn(nx::format("invalid group count set: %i", N));
                            std::cout << "Ошибка: значение должно находиться в интервале от 1 до 1000" << std::endl;
                            continue;
                        }
                        break;
                    }while (true);
                    log_info(nx::format("group count is set to %i", N));
                    break;
                }
                case 5: {
                    log_info("generate new data file option is chosen");
                    std::string filename;
                    std::cout << "Введите название: ";
                    std::cin >> filename;
                    generate_new_input_file(filename);
                    break;
                }
                case 1: {
                    log_info("run sorting algorithm option chosen");
                    check_input_file_exists(input_file);
                    read_source(input_file);
                    merge_source();
                    split_source_into_groups(N);
                    sort_sources_groups();
                    check_groups();
                    write_groups(output_file);
                    break;
                }
                default: {
                    std::cout << "Некорректный номер команды" << std::endl;
                    log_info ("invalid command entered");
                    break;
                }
            }
        }
    }

    void diamond_engine::read_source(const marker_string&p_filename)
    {
        log_info ("starting...");
        log_info (nx::format("attempting to read file %s", p_filename.c_str()));
        std::ifstream fin(p_filename.c_str());
        if (!fin)
        {
            std::cerr << "File " << p_filename << " cannot be opened for read" << std::endl;
            log_error(nx::format("failure opening file '%s'. aborting", p_filename.c_str()));
            return;
        }
        log_info (nx::format("file '%s' is open for reading", p_filename.c_str()));
        // log_info (nx::format("clearing existing sources (%i elements)", _sources.size()));
        // _sources.clear();
        // _max_size = 0;
        // _min_size = 100000;

        log_info (nx::format("clearing existing mines (%i elements)", _mines.size()));
        _mines.clear();


        std::string line;
        std::string cur_company { "unknown" };
        int counter = 0;
        double cur_size = 0;
        size_t pos = 0;

        while (std::getline(fin, line) && ++counter) {
            if (line.empty()) {
                log_warn(nx::format("parsing warning: line %i is empty", counter));
                continue;
            }
            if (line.ends_with(':')) {
                cur_company = line.substr(0, line.size() - 1);
                log_info(nx::format("reading data for %s company...", cur_company.c_str()));
                _mines.push_front({cur_company});
                continue;
            }
            cur_size = std::stod(line.c_str(), &pos);
            if (pos == 0) {
                log_warn(nx::format("cannot process line %i: %s", counter, line.c_str()));
                continue;
            }
            // _max_size = cur_size > _max_size ? cur_size : _max_size;
            // _min_size = cur_size < _min_size ? cur_size : _min_size;
            // _sources.push_front({cur_company.c_str(), cur_size});
            _mines.at(0).diamonds.push_front(cur_size);
        }

        std::cout << "[diamond_engine][read_source] : read sources for "
        << _mines.size() << " companies from '" << p_filename << "'" << std::endl;
        log_info(nx::format("read %i sources from file '%s'", _mines.size(), p_filename.c_str()));
        log_info("finished successfully");
    }

    void diamond_engine::merge_source() {
        log_info ("starting...");
        log_info (nx::format("clearing existing sources (%i elements)", _sources.size()));
        _sources.clear();

        _max_size = 0;
        _min_size = 100000;

        for (auto & mine: _mines) {
            for (auto diamond: mine.diamonds) {
                _max_size = diamond > _max_size ? diamond : _max_size;
                _min_size = diamond < _min_size ? diamond : _min_size;
                _sources.push_front({mine.mine_name, diamond});
            }
        }

        log_info(nx::format("determined maximum and minimum sizes: max=%f, min=%f", _max_size, _min_size));
        log_info(nx::format("merged %i entries in total", _sources.size()));
        log_info("finished successfully");
    }

    void diamond_engine::split_source_into_groups(unsigned p_group_count)
    {
        log_info ("starting...");
        log_info ("clearing existing diamond groups");
        _groups.clear();
        log_info (nx::format("allocating memory for %i diamond groups...", p_group_count));
        for (int i = 0; i < p_group_count; i++) {
            double lower_bound = (_max_size - _min_size)/p_group_count * i + _min_size;
            double upper_bound = (_max_size - _min_size)/p_group_count * (i + 1) + _min_size;
            _groups.push_front({lower_bound, upper_bound});
        }
        _groups.reverse();
        log_info (nx::format("begin going through sources"));
        for (auto it = _sources.begin(); it != _sources.end(); ++it)
        {
            int s = it->size;
            int a = (_max_size - _min_size);
            int b = (int)((s - _min_size)) * p_group_count;
            int group_index = (b - 1)/a;
            _groups.at(group_index).sources.push_front(*it);
        }
        std::cout << "[diamond_engine][split_source_into_groups] : split sources into "
        << p_group_count << " groups" << std::endl;
        for (int i = 0; i < _groups.size(); ++i) {
            std::cout << "\tgroup " << i + 1 << ": " << _groups.at(i).sources.size() << " elements" << std::endl;
        }
        log_info (nx::format("successfully divided sources into %i groups", p_group_count));
     }

    void diamond_engine::sort_sources_groups()
    {
        log_info("starting...");
        log_info("going through diamond groups...");
        for (int i = 0; i < _groups.size(); ++i)
        {
            std::cout << "[diamond_engine][sort_sources_groups] : sorting elements in group "
            << i + 1 << "... " << std::flush;
            log_info(nx::format("begin sorting group %i (%i elements)", i + 1, _groups.at(i).sources.size()));
            _groups.at(i).sources.sort([] (const diamond_source & first, const diamond_source & second) -> bool {
                return first.size < second.size;
            });
            std::cout << "finished" << std::endl;
            log_info(nx::format("finished sorting group %i", i + 1));
        }
        std::cout << "[diamond_engine][sort_sources_groups] : finished sorting" << std::endl;
        log_info("finished successfully");
    }

    void diamond_engine::print_sources_groups()
    {
        log_info ("starting...");
        log_info ("going through diamond groups...");
        int i = 0;
        for (auto it = _groups.begin(); it != _groups.end(); ++it, ++i)
        {
            auto & group = *it;;
            std::cout << "Group " << i + 1 << " (" << group.lower_bound << "-" << group.upper_bound << ")"
                << "[" << group.sources.size() << " elements]:" << std::endl;
            for (auto src: group.sources)
            {
                std::cout << "\t" << std::setw(30) << std::setfill('-') <<  src.company_name << " : " << src.size << std::endl;
            }
            log_info(nx::format("printed group %1", i + 1));
        }
        log_info ("finished successfully");
    }

    void diamond_engine::write_groups(const marker_string &p_filename) {
        log_info("starting...");
        log_info(nx::format("opening output file '%s'", p_filename.c_str()));
        std::ofstream fout(p_filename.c_str());
        if (!fout)
        {
            std::cerr << "[diamond_engine][write_groups] : failed to open output file '" << p_filename << "'" << std::endl;
            log_error(nx::format("failure while opening output file '%s'. aborting", p_filename.c_str()));
            return;
        }

        log_info("begin going through initial data mines...");
        fout << "Initial data: " << std::endl;
        for (auto & mine: _mines) {
            fout << mine.mine_name << ": " << std::endl;
            for (auto diamond: mine.diamonds) {
                fout << "\t" << diamond << std::endl;
            }
        }
        log_info("finished writing initial data into result file");

        fout << std::endl << "Result: " << std::endl;
        log_info("begin going though diamond groups...");
        int i = 0;
        for (int i = 0; i < _groups.size(); ++i)
        {
            auto & group = _groups.at(i);
            int gr_size = group.sources.size();
            // fout << "Group " << i + 1 << " (" << group.sources.size()
            // << (group.size() == 1 ? " element" : " elements") << "):" << std::endl;
            fout << "Group " << i + 1 << " (" << group.lower_bound << "-" << group.upper_bound << ")"
            << "[" << group.sources.size() << " elements]:" << std::endl;
            log_info(nx::format("writing group %i (%i elements)...", i + 1, group.sources.size()));
            for (auto src: group.sources)
            {
                fout << "\t" << src.size << " --- " << src.company_name << std::endl;
            }
            std::cout << "[diamond_engine][write_groups] : wrote group " << i + 1 << " to output file" << std::endl << std::flush;
            log_info(nx::format("finished writing group %i", i + 1));
        }
        log_info("closing output file");
        fout.close();
        std::cout << "[diamond_engine][write_groups] : finished writing to output file '" << p_filename << "'" << std::endl;
        log_info(nx::format("finished successfully (wrote %i groups in total)", _groups.size()));
    }

    void diamond_engine::check_groups()
    {
        log_info("starting...");
        std::cout << "[diamond_engine][check_groups] : begin checking sorted groups" << std::endl;
        int counter = 1;
        bool all_ok = true;
        log_info("begin going through diamond groups...");
        for (auto gr_it = _groups.begin(); gr_it != _groups.end(); ++gr_it, counter++)
        {
            std::cout << "[diamond_engine][check_groups] : checking group " << counter << "... ";
            log_info(nx::format("checking group %i", counter));
            // double min = gr_it->begin()->size;
            double min = 0;
            bool group_ok = true;
            for (auto it = gr_it->sources.begin(); it != gr_it->sources.end(); ++it)
            {
                if (min > it->size)
                {
                    std::cerr << "[diamond_engine][check_groups] : group " << counter << " failed the check" << std::endl;
                    log_error(nx::format("group %i failed the check. Elements are not sorted in ascending order", counter));
                    group_ok = false;
                    all_ok = false;
                    break;
                }
                min = it->size;
            }
            if (group_ok) { std::cout << "passed" << std::endl; }
            else std::cout << "failed" << std::endl;
        }
        if (all_ok) {
            std::cout << "[diamond_engine][check_groups] : check finished successfully" << std::endl;
            log_info("finished successfully");
        }
        else {
            std::cout << "[diamond_engine][check_groups] : check failed" << std::endl;
            log_error("check failed");
        }
    }


    int diamond_engine::exec()
    {
        init();
        main_loop();
        log_info("exiting with code 0");
        return 0;
    }

    bool diamond_engine::create_source(const marker_string &p_filename, uint32_t p_entries) {
        std::cout << "[diamond_engine][create_source] : starting..." << std::endl;
        log_info("starting...");
        log_info("allocating temporal memory for storing data...");
        nx::forward_list<nx::forward_list<double>> diamonds_in_mines;
        // diamonds_in_mines.resize(diamond_companies.size());
        for (int i = 0; i < diamond_companies.size(); ++i) {diamonds_in_mines.push_front({}); }
        uint32_t mine_id = 0;
        double size;
        for (int i = 0; i < p_entries; ++i) {
            mine_id = rand() % diamonds_in_mines.size();
            size = get_random_diamond_size(10, 1000);
            diamonds_in_mines.at(mine_id).push_front(size);
        }
        uint64_t total_memory = diamonds_in_mines.size() * sizeof(nx::forward_list<double>); // size of lists
        total_memory += p_entries * sizeof(double); // size of data in lists
        log_info(nx::format("total memory allocated = %i byte", total_memory));
        log_info(nx::format("attemting to write data to file '%s'...", p_filename.c_str()));
        std::ofstream fout(p_filename.c_str());
        if (!fout) {
            std::cerr << "[diamond_engine][create_source] : failed to open file '" << p_filename << "'" << std::endl;
            log_error(nx::format("failed to open file '%s' for write. aborting", p_filename.c_str()));
            return false;
        }
        log_info("going through data...");
        int i = 0;
        for (auto it = diamonds_in_mines.begin(); it != diamonds_in_mines.end(); ++it, ++i) {
            if (it->empty())
                continue;
            fout << diamond_companies.at(i) << ":" << std::endl;
            for (auto s: *it)
                fout << s << std::endl;
            log_info(nx::format("wrote entries for '%s'", diamond_companies.at(i).c_str()));
        }
        fout.close();
        std::cout << "[diamond_engine][create_source] : wrote " << p_entries << " entries into '" << p_filename << "'" << std::endl;
        log_info(nx::format("successfully generated source file (%i entries written)", p_entries));
        return true;
    }
} // diamond