#include "main.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>


#include "AIXI/agent.hpp"
#include "environments/environment.hpp"
#include "MCTS/search.hpp"
#include "common/util.hpp"
#include "common/types.hpp"

// Streams for logging
std::ofstream logger;        // A verbose human-readable log
std::ofstream compactLog; // A compact comma-separated value log

// The main agent/environment interaction loop
void mainLoop(Agent &ai, Environment &env, options_t &options) {

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

	bool tree_initialized = false;

	percept_t observation;
	percept_t reward;
	action_t action;

	while (ai.historySize() <= strExtract<unsigned int>(options["ct-depth"])) {
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
		std::cout << "Cycle "  << cycle  << " , Obs " << observation << " , Rew " << reward << std::endl;
		// Update agent's environment model with the new percept
		ai.modelUpdate(observation, reward); // TODO: implement in agent.cpp		
		// Determine best exploitive action, or explore
		bool explored = false;
		if (explore && rand01() < explore_rate) {
			std::cout << "exploring" << std::endl;
			explored = true;
			action = ai.genRandomAction();
		} else {			
			action = search(ai, observation, reward, action, tree_initialized);
			tree_initialized = true;
		}

		// Send an action to the environment
		env.performAction(action); // TODO: implement for each environment

		// Update agent's environment model with the chosen action		
		ai.modelUpdate(action); // TODO: implement in agent.cpp		


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

	}

	// Print summary to standard output
	std::cout << std::endl << std::endl << "SUMMARY" << std::endl;
	std::cout << "agent lifetime: " << ai.lifetime() << std::endl;
	std::cout << "average reward: " << ai.averageReward() << std::endl;
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

int main(int argc, char *argv[]) {
	if (argc < 2 || argc > 3) {
		std::cerr << "ERROR: Incorrect number of arguments" << std::endl;
		std::cerr << "The first argument should indicate the location of the configuration file and the second (optional) argument should indicate the file to log to." << std::endl;
		return -1;
	}

	// Set up logging
	std::string log_file = argc < 3 ? "log" : argv[2];
	logger.open((log_file + ".log").c_str());
	compactLog.open((log_file + ".csv").c_str());

	// Print header to compactLog
	compactLog << "cycle, observation, reward, action, explored, explore_rate, total reward, average reward" << std::endl;


	// Load configuration options
	options_t options;

	// Default configuration values
	options["ct-depth"] = "3";
	options["agent-horizon"] = "9";
	options["exploration-exploitation-ratio"] = "1";
	options["num-simulations"] = "500";
	options["exploration"] = "0";     // do not explore
	options["explore-decay"] = "1.0"; // exploration rate does not decay

	// Read configuration options
	std::ifstream conf(argv[1]);
	if (!conf.is_open()) {
		std::cerr << "ERROR: Could not open file '" << argv[1] << "' now exiting" << std::endl;
		return -1;
	}
	processOptions(conf, options);
	conf.close();

	// Set up the environment
	Environment *env;

	// TODO: instantiate the environment based on the "environment-name"
	// option. For any environment you do not implement you may delete the
	// corresponding if statement.
	// NOTE: you may modify the options map in order to set quantities such as
	// the reward-bits for each particular environment. See the coin-flip
	// experiment for an example.
	std::string environment_name = options["environment"];
	if (environment_name == "coin-flip") {
		env = new CoinFlip(options);
		options["agent-actions"] = "2";
		options["observation-bits"] = "1";
		options["reward-bits"] = "1";
	}
	else if (environment_name == "ctw-test") {
		env = new CTWTest(options);
		options["agent-actions"] = "2";
		options["observation-bits"] = "1";
		options["reward-bits"] = "1";
	}
	else if (environment_name == "1d-maze") {
		// TODO: instantiate "env" (if appropriate)
	}
	else if (environment_name == "cheese-maze") {
		// TODO: instantiate "env" (if appropriate)
	}
	else if (environment_name == "tiger") {
		// TODO: instantiate "env" (if appropriate)
	}
	else if (environment_name == "extended-tiger") {
		// TODO: instantiate "env" (if appropriate)
		env = new ExtendedTiger(options);
		options["agent-actions"] = "4";
		options["observation-bits"] = "3";
		options["reward-bits"] = "7";
	}
	else if (environment_name == "4x4-grid") {
		// TODO: instantiate "env" (if appropriate)
	}
	else if (environment_name == "tictactoe") {
		// TODO: instantiate "env" (if appropriate)
		env = new TicTacToe(options);
		options["agent-actions"] = "9";
		options["observation-bits"] = "18";
		options["reward-bits"] = "4";
	}
	else if (environment_name == "biased-rock-paper-scissor") {
		// TODO: instantiate "env" (if appropriate)
		env = new BiasedRockPaperSciessor(options);
		options["agent-actions"] = "2";
		options["observation-bits"] = "4";
		options["reward-bits"] = "3";
	}
	else if (environment_name == "kuhn-poker") {
		// TODO: instantiate "env" (if appropriate)
		env = new KuhnPoker(options);
		options["agent-actions"] = "2";
		options["observation-bits"] = "4";
		options["reward-bits"] = "4";
	}
	else if (environment_name == "pacman") {
		// TODO: instantiate "env" (if appropriate)
		env = new Pacman(options);
		options["agent-actions"] = "4";
		options["observation-bits"] = "16";
		options["reward-bits"] = "8";
	}
	else if (environment_name == "ctwtest") {
		// TODO: instantiate "env" (if appropriate)
		env = new CTWTest(options);
		options["agent-actions"] = "2";
		options["observation-bits"] = "1";
		options["reward-bits"] = "1";
	}
	else {
		std::cerr << "ERROR: unknown environment '" << environment_name << "'" << std::endl;
		return -1;
	}

	// Set up the agent
	Agent ai(options);

	// Run the main agent/environment interaction loop
	mainLoop(ai, *env, options);

	logger.close();
	compactLog.close();

	return 0;
}
