//
// Created by nexie on 24.06.2025.
//

#include "marker_string.h"

marker_string remove_special_symbols_from_word_n(const marker_string & in, int n) {
    marker_string out {in};
    int i = 0;
    bool current_is_word = (*in.data() != ' ');
    int word_count = current_is_word ? 1 : 0;
    for (char * ch = in.data(); *ch != in.marker(); ch++) {
        if (*ch != ' ' && (current_is_word == false)) {
            word_count++;
            current_is_word = true;
        }
        else if (*ch == ' ') current_is_word = false;
        else current_is_word = true;
        if (*ch == ' ') {
            out.data()[i++] = *ch;
            continue;
        }
        if (word_count == n) {
            if ((*ch >= 'a' && *ch < 'z') || (*ch >= 'A' && *ch <= 'Z'))
                out.data()[i++] = *ch;
        }
        else
            out.data()[i++] = *ch;
    }
    out.data()[i] = out.marker();
    return out;
}
