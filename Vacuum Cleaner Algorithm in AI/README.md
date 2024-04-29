# Vacuum Cleaner Algorithm in AI

This repository contains Python code for a vacuum cleaner algorithm implemented using reinforcement learning, specifically Deep Q-Networks (DQN). The algorithm is designed to enable a virtual vacuum cleaner agent to learn how to efficiently clean a grid-based environment with randomly distributed dirt.

## Contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Usage](#usage)
- [License](#license)

## Overview

The implementation consists of two main components:

1. **Vacuum Cleaner Environment (VacuumCleanerEnv)**:
   - This class defines the environment in which the vacuum cleaner agent operates.
   - The environment is a grid of a specified size, where each cell can either be clean or dirty.
   - The agent can move in four directions (left, right, up, down) and can also perform the "suck" action to clean the dirt in the current cell.
   - The environment provides observations of the grid state, rewards for cleaning dirt, and signals when the cleaning task is completed.

2. **DQN Agent (DQNAgent)**:
   - This class implements the Deep Q-Learning algorithm for training the vacuum cleaner agent.
   - The agent learns a policy to decide actions based on observations of the environment.
   - It maintains a memory buffer of experiences (state, action, reward, next state) and updates its Q-values through iterative training.
   - The agent's Q-network consists of fully connected layers and utilizes the Adam optimizer for training.

## Requirements

- Python 3.x
- TensorFlow
- Gym (OpenAI)
- NumPy

## Usage

Run the main script:

   ```bash
   python vacuum_cleaner_algorithm.py
   ```

   This will train the DQN agent to clean the grid environment for a specified number of episodes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
