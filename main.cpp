#include "main.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include <cstdlib>
#include <time.h>

#include <algorithm>


#include "AIXI/agent.hpp"
#include "environments/environment.hpp"
#include "MCTS/search.hpp"
#include "common/util.hpp"
#include "common/types.hpp"

// Streams for logging
std::ofstream logger;        // A verbose human-readable log
std::ofstream compactLog; // A compact comma-separated value log
std::ofstream eval_logger;        // SeparateLogger for evaluation

void evalLoop(Agent &ai, Environment &env, options_t &options, int cycles, int phase);

// The main agent/environment interaction loop
void mainLoop(Agent &ai, Environment &env, Environment &xd_env, options_t &options) {

	int eval_cycles,eval_freq,mid_eval_val, finaleval_cycles;
	bool isMidEvalEnabled=false;
	// Evaluation details
	strExtract(options["mideval-cycles"], eval_cycles);
	strExtract(options["mideval-frequency"], eval_freq);
	strExtract(options["mideval-enable"], mid_eval_val);
	strExtract(options["final-eval-cycles"], finaleval_cycles);	
	isMidEvalEnabled = mid_eval_val==1;

	// Determine exploration options
	bool explore = options.count("exploration") > 0;
	double explore_rate, explore_decay;
	if (explore) {
		strExtract(options["exploration"], explore_rate);
		strExtract(options["explore-decay"], explore_decay);
		assert(0.0 <= explore_rate && explore_rate <= 1.0);
		assert(0.0 <= explore_decay && explore_decay <= 1.0);
	}

	// Determine termination lifetime
	bool terminate_check = options.count("terminate-lifetime") > 0;
	lifetime_t terminate_lifetime;
	if (terminate_check) {
		strExtract(options["terminate-lifetime"], terminate_lifetime);
		assert(0 <= terminate_lifetime);
	}

	percept_t observation;
	percept_t reward;
	action_t action;
	// Initial cycles to get things going.
	while (ai.historySize() <= strExtract<unsigned int>(options["ct-depth"])) {
		std::cout << "initialize context" << std::endl;
		observation = env.getObservation();
		reward = env.getReward();
		action = ai.genRandomAction();
		ai.modelUpdate(observation, reward);
		ai.modelUpdate(action);
	}

        // Agent/environment interaction loop
	for (unsigned int cycle = 1; !env.isFinished(); cycle++) {

		// check for agent termination
		if (terminate_check && ai.lifetime() > terminate_lifetime) {
			logger << "info: terminating lifetiment" << std::endl;
			break;
		}

		// Get a percept from the environment
		observation = env.getObservation();
		reward = env.getReward();
		std::cout << "Cycle "  << cycle  << " , Obs " << observation << " , Rew " << reward << " , Exp rate " << explore_rate << std::endl;
		// Update agent's environment model with the new percept
		ai.modelUpdate(observation, reward);
		// Determine best exploitive action, or explore

		bool explored = false;
		if (explore && rand01() < explore_rate) {
			std::cout << "exploring" << std::endl;
			explored = true;
			action = ai.genRandomAction();
		} else {
			action = ai.getPlannedAction(observation, reward, action);
		}

		// Send an action to the environment
		env.performAction(action);

		// Update agent's environment model with the chosen action
		ai.modelUpdate(action);


		// Log this turn
		logger << "cycle: " << cycle << std::endl;
		logger << "observation: " << observation << std::endl;
		logger << "reward: " << reward << std::endl;
		logger << "action: " << action << std::endl;
		logger << "explored: " << (explored ? "yes" : "no") << std::endl;
		logger << "explore rate: " << explore_rate << std::endl;
		logger << "total reward: " << ai.reward() << std::endl;
		logger << "average reward: " << ai.averageReward() << std::endl;

		// Log the data in a more compact form
		compactLog << cycle << ", " << observation << ", " << reward << ", "
				<< action << ", " << explored << ", " << explore_rate << ", "
				<< ai.reward() << ", " << ai.averageReward() << std::endl;

		// Print to standard output when cycle == 2^n
		if ((cycle & (cycle - 1)) == 0) {
			std::cout << "cycle: " << cycle << std::endl;
			std::cout << "average reward: " << ai.averageReward() << std::endl;
			if (explore) {
				std::cout << "explore rate: " << explore_rate << std::endl;
			}
		}

		// Update exploration rate
		if (explore) explore_rate *= explore_decay;

		ai.incAgentAge();

		if(isMidEvalEnabled && cycle%eval_freq == 0){
			if(&xd_env == nullptr)
				evalLoop(ai, env, options, eval_cycles, (int)cycle/eval_freq);
			else
				evalLoop(ai, xd_env, options, eval_cycles, (int)cycle/eval_freq);
		}
	}

	// Print summary to standard output
	std::cout << std::endl << std::endl << "Training SUMMARY" << std::endl;
	std::cout << "agent lifetime: " << ai.lifetime() << std::endl;
	std::cout << "average reward: " << ai.averageReward() << std::endl;

	logger << "info: Starting evaluation." << std::endl;

	if(&xd_env == nullptr)
		evalLoop(ai, env, options, finaleval_cycles, 1);
	else
		evalLoop(ai, xd_env, options, finaleval_cycles, 1);
}


void evalLoop(Agent &ai, Environment &env, options_t &options, int cycles, int phase){

	percept_t observation;
	percept_t reward;
	action_t action;

	percept_t eval_tot_reward = 0.0;

	for (unsigned int cycle = cycles*(phase-1)+1; cycle<=cycles*phase; cycle++) {

		// Get a percept from the environment
		observation = env.getObservation();
		reward = env.getReward();
		std::cout << "Evaluation Cycle "  << cycle  << " , Obs " << observation << " , Rew " << reward << std::endl;

		eval_tot_reward += reward;

		// Update agent's environment model with the new percept
		ai.modelUpdate(observation, reward);

		// Determine best exploitive action
		action = ai.getPlannedAction(observation, reward, action);

		// Send an action to the environment
		env.performAction(action);

		// Update agent's environment model with the chosen action
		ai.modelUpdate(action);

		eval_logger << cycle << ", " << reward << ", " << eval_tot_reward/(double)cycle << std::endl;

		// Print to standard output when cycle == 2^n
		if ((cycle & (cycle - 1)) == 0) {
			std::cout << "Evaluation cycle: " << cycle << std::endl;
			std::cout << "average reward: " << eval_tot_reward/(double)cycle << std::endl;
		}
	}

	std::cout << std::endl << std::endl << "Evaluation SUMMARY" << std::endl;
	std::cout << "Cycles Evaluated: " << cycles << std::endl;
	std::cout << "Average Reward per cycle: " << eval_tot_reward/(double)cycles << std::endl;

	logger << "Evaluation SUMMARY" << std::endl;
	logger << "Cycles Evaluated: " << cycles << std::endl;
	logger << "Average Reward per cycle: " << eval_tot_reward/(double)cycles << std::endl;
}


// Populate the 'options' map based on 'key=value' pairs from an input stream
void processOptions(std::ifstream &in, options_t &options) {
	std::string line;
	size_t pos;

	for (int lineno = 1; in.good(); lineno++) {
		std::getline(in, line);

		// Ignore # comments
		if ((pos = line.find('#')) != std::string::npos) {
			line = line.substr(0, pos);
		}

		// Remove whitespace
		while ((pos = line.find(" ")) != std::string::npos)
			line.erase(line.begin() + pos);
		while ((pos = line.find("\t")) != std::string::npos)
			line.erase(line.begin() + pos);


		// Split into key/value pair at the first '='
		pos = line.find('=');
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		// Check that we have parsed a valid key/value pair. Warn on failure or
		// set the appropriate option on success.
		if (pos == std::string::npos) {
			std::cerr << "WARNING: processOptions skipping line " << lineno << " (no '=')" << std::endl;
		}
		else if (key.size() == 0) {
			std::cerr << "WARNING: processOptions skipping line " << lineno << " (no key)" << std::endl;
		}
		else if (value.size() == 0) {
			std::cerr << "WARNING: processOptions skipping line " << lineno << " (no value)" << std::endl;
		}
		else {
			options[key] = value; // Success!
			std::cout << "OPTION: '" << key << "' = '" << value << "'" << std::endl;
		}

	}
}

class CmdLineParser {
 private:
  std::vector<std::string> options;

 public:
  CmdLineParser(int& argc, char** argv) {
    for (int i = 1; i < argc; ++i)
      this->options.push_back(std::string(argv[i]));
  }
  const std::string& getFlagValue(const std::string& flag) const {
    std::vector<std::string>::const_iterator itr;
    itr = std::find(this->options.begin(), this->options.end(), flag);
    if (itr != this->options.end() && ++itr != this->options.end()) {
      return *itr;
    }
    static const std::string empty_string("");
    return empty_string;
  }
  bool flagExists(const std::string& option) const {
    return std::find(this->options.begin(), this->options.end(), option) !=
           this->options.end();
  }
};

void printUsage(){
	std::cout << "Usage:" << std::endl;
	std::cout << "./bin/aixi -c domain_config_file [-x cross_domain_name] [-l log_file_prefix]" << std::endl;
	std::cout << "./bin/aixi -h" << std::endl;
	std::cout << "-h : Prints this Help Message" << std::endl;
	std::cout << "-c <file>: Tells AIXI which domain and configuration to use. (Mandatory)" << std::endl;
	std::cout << "-x <file>: Evaluate in a different domain. (Optional)" << std::endl;
	std::cout << "\tValid Domain Names: coin-flip, extended-tiger, tictactoe, biased-rock-paper-scissor, kuhn-poker, pacman." << std::endl;
	std::cout << "-l <log file prefix>: Give a prefix to the output log file. Useful when running multiple trials. (Optional)" << std::endl;	
}

Environment* environmentFactory(std::string environment_name, options_t& options, bool only_env){

	//std::string environment_name = options["environment"];
	if (environment_name == "coin-flip") {
		if(!only_env){
			options["agent-actions"] = "2";
			options["observation-bits"] = "1";
			options["reward-bits"] = "1";
		}
		return new CoinFlip(options);
	}
	else if (environment_name == "ctw-test") {
		if(!only_env){
			options["agent-actions"] = "2";
			options["observation-bits"] = "1";
			options["reward-bits"] = "1";
		}
		return new CTWTest(options);
	}
	else if (environment_name == "extended-tiger") {
		if(!only_env){		
			options["agent-actions"] = "4";
			options["observation-bits"] = "3";
			options["reward-bits"] = "7";
		}
		return new ExtendedTiger(options);
	}
	else if (environment_name == "tictactoe") {
		if(!only_env){
			options["agent-actions"] = "9";
			options["observation-bits"] = "18";
			options["reward-bits"] = "4";
		}
		return new TicTacToe(options);
	}
	else if (environment_name == "biased-rock-paper-scissor") {
		if(!only_env){	
			options["agent-actions"] = "3";
			options["observation-bits"] = "2";
			options["reward-bits"] = "2";
		}
		return new BiasedRockPaperSciessor(options);
	}
	else if (environment_name == "kuhn-poker") {
		if(!only_env){
			options["agent-actions"] = "2";
			options["observation-bits"] = "4";
			options["reward-bits"] = "4";
		}
		return new KuhnPoker(options);
	}
	else if (environment_name == "pacman") {
		if(!only_env){
			options["agent-actions"] = "4";
			options["observation-bits"] = "16";
			options["reward-bits"] = "8";
		}
		return new Pacman(options);
	}
	else if (environment_name == "ctwtest") {
		if(!only_env){
			options["agent-actions"] = "2";
			options["observation-bits"] = "1";
			options["reward-bits"] = "1";
		}
		return new CTWTest(options);
	}
	else {
		std::cerr << "ERROR: unknown environment '" << environment_name << "'" << std::endl;
		return NULL;
	}
}

int main(int argc, char *argv[]) {

	CmdLineParser cl_options(argc, argv);

    if(cl_options.flagExists("-h")){
		printUsage();
		return 0;
    }

	if(!cl_options.flagExists("-c")){
		std::cerr << "ERROR: Incorrect number of arguments" << std::endl;
		std::cerr << "The -c <configuration file> argument is mandatory. It indicates the location of the configuration file." << std::endl << std::endl;
		printUsage();
		return -1;
	}


	//initialize random seed
	srand(time(NULL));

	options_t options;

	// Default configuration values
	options["ct-depth"] = "7";
	options["agent-horizon"] = "3";
	options["exploration-exploitation-ratio"] = "1.4";
	options["num-simulations"] = "3";
	options["exploration"] = "0";     // do not explore
	options["explore-decay"] = "1.0"; // exploration rate does not decay
	options["mideval-cycles"] = "50"; // number of cycles to evaluate
	options["mideval-frequency"] = "50"; // in how many cycles eval to be triggered
	options["mideval-enable"] = "0"; //Enable eval during middle of trining. 0:False, 1:True
	options["final-eval-cycles"] = "5000"; //Enable eval during middle of trining. 0:False, 1:True

	// Read configuration options
	std::ifstream conf(cl_options.getFlagValue("-c"));
	if (!conf.is_open()) {
		std::cerr << "ERROR: Could not open file '" << cl_options.getFlagValue("-c") << "' now exiting" << std::endl;
		return -1;
	}
	processOptions(conf, options);
	conf.close();

	std::string environment_name = options["environment"];

	// Set up logging
	std::string log_file = cl_options.flagExists("-l")? cl_options.getFlagValue("-l"): "log";
	logger.open((log_file +"_" + environment_name + ".log").c_str());
	compactLog.open((log_file + "_" + environment_name + ".csv").c_str());
	eval_logger.open((log_file +"_" + environment_name + "_eval.log").c_str());

	// Print header to compactLog
	compactLog << "cycle, observation, reward, action, explored, explore_rate, total reward, average reward" << std::endl;


	// Load configuration options
	

	// Set up the environment
	Environment *env, *xd_env=nullptr;
	env = environmentFactory(environment_name, options, false);

	if(cl_options.flagExists("-x")) {
		xd_env = environmentFactory(cl_options.getFlagValue("-x"), options, true);
	}


	// Set up the agent
	Agent ai(options);
	ai.initPlanner();
	// Run the main agent/environment interaction loop
	mainLoop(ai, *env, *xd_env, options);

	logger.close();
	compactLog.close();
	eval_logger.close();


	if(cl_options.flagExists("-x")){
		logger.open((log_file +"_" + environment_name + "_xd_" + cl_options.getFlagValue("-x") + ".log").c_str());
		compactLog.open((log_file + "_" + environment_name + "_xd_" + cl_options.getFlagValue("-x") + ".csv").c_str());
		eval_logger.open((log_file +"_" + environment_name + "_xd_" + cl_options.getFlagValue("-x") + "_eval.log").c_str());
		xd_env=nullptr;
		lifetime_t terminate_lifetime;
		strExtract(options["terminate-lifetime"], terminate_lifetime);
		options["terminate-lifetime"] = 2*terminate_lifetime;
		mainLoop(ai, *env, *xd_env, options);
		logger.close();
		compactLog.close();
		eval_logger.close();		
	}		

	return 0;
}
