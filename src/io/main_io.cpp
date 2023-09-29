#include "main_io.h"

#include "lib/logger/logger.h"

Options::Options() : output_name_(NULL), input_name_(NULL) {}

Options::Options(const Options& options)
    : output_name_(options.output_name_), input_name_(options.input_name_) {}

Options::~Options() {}

Options& Options::operator=(const Options& options) {
    input_name_ = options.input_name_;
    output_name_ = options.output_name_;
    return *this;
}

const char* Options::get_output_name() { return output_name_; }
const char* Options::get_input_name() { return input_name_; }

void Options::set_output_name(const char* new_name) { output_name_ = new_name; }
void Options::set_input_name(const char* new_name) { input_name_ = new_name; }

static const char OWL_TEXT[] = R"""(You let the owls out!
    A_,,,_A        A_,,,_A        A_,,,_A    
   ((O)V(O))      ((O)V(O))      ((O)V(O))   
  ("\"|"|"/")    ("\"|"|"/")    ("\"|"|"/")  
   \"|"|"|"/      \"|"|"|"/      \"|"|"|"/   
     "| |"          "| |"          "| |"     
      ^ ^            ^ ^            ^ ^      )""";

error_t parse(int key, char* arg, struct argp_state* state) {
    Options* options = (Options*)state->input;

    switch (key) {
        case 'o':
            options->set_output_name(arg);
            break;
        case OPT_OWL:
            puts(OWL_TEXT);
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num == 0) options->set_input_name(arg);
            break;
        case ARGP_KEY_END:
            if (state->arg_num == 0) {
                log_dup(ERROR_REPORTS, "error",
                        "Input file path was not specified.\n");
                errno = ENODATA;
                return ARGP_ERR_UNKNOWN;
            }
            break;
        default:
            break;
    }

    return 0;
}
