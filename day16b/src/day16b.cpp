#include <bitset>
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

static const size_t MAX_TRANSMISSION_BITS = 10000;

struct Packet {
	unsigned long version;
	unsigned long type_id;
	stack<unsigned long> values;
};

class Transmission {
	bitset<MAX_TRANSMISSION_BITS> transmission;
	size_t transmission_length = 0;
	stack<string> expression;

	unsigned long evaluate(Packet &packet) {
		unsigned long result;

		switch (packet.type_id) {

			case 0: {
				// Add up all values
				unsigned long sum = 0;
				while (!packet.values.empty()) {
					sum += packet.values.top();
					packet.values.pop();
				}
				result = sum;
			} break;

			case 1: {
				// Multiply all values
				unsigned long product = 1;
				while (!packet.values.empty()) {
					product *= packet.values.top();
					packet.values.pop();
				}
				result = product;
			} break;

			case 2: {
				// Take minimum value
				unsigned long minimum = (unsigned long)-1;
				while (!packet.values.empty()) {
					minimum = min(minimum, packet.values.top());
					packet.values.pop();
				}
				result = minimum;
			} break;

			case 3: {
				// Take maximum value
				unsigned long maximum = 0;
				while (!packet.values.empty()) {
					maximum = max(maximum, packet.values.top());
					packet.values.pop();
				}
				result = maximum;
			} break;

			case 4: {
				unsigned long value = packet.values.top();
				packet.values.pop();
				result = value;
			} break;

			case 5: {
				// 1 if first value is greater than second value, otherwise 0
				unsigned long second_value = packet.values.top();
				packet.values.pop();
				unsigned long first_value = packet.values.top();
				packet.values.pop();
				result = (first_value > second_value) ? 1: 0;
			} break;

			case 6: {
				// 1 if first value is less than second value, otherwise 0
				unsigned long second_value = packet.values.top();
				packet.values.pop();
				unsigned long first_value = packet.values.top();
				packet.values.pop();
				result = (first_value < second_value) ? 1: 0;
			} break;

			case 7: {
				// 1 if first value is equal to second value, otherwise 0
				unsigned long second_value = packet.values.top();
				packet.values.pop();
				unsigned long first_value = packet.values.top();
				packet.values.pop();
				result = (first_value == second_value) ? 1: 0;
			} break;

			default:
				cout << "HUH!?" << endl;
		}

		return result;
	}

	void processLiteralValue(size_t &pos, Packet &packet) {
		unsigned long literal = 0;
		bitset<5> five_bits;
		bool last_group = false;

		// Read literal
		while (!last_group) {
			five_bits.set(4, this->transmission[pos++]);
			five_bits.set(3, this->transmission[pos++]);
			five_bits.set(2, this->transmission[pos++]);
			five_bits.set(1, this->transmission[pos++]);
			five_bits.set(0, this->transmission[pos++]);

			last_group = !five_bits[4];
			five_bits.set(4, false);
			literal = literal << 4;
			literal += five_bits.to_ulong();
		}

		packet.values.push(literal);
	}

	void processSubpacketsByCount(size_t &pos, Packet &packet) {
		bitset<11> eleven_bits;

		// The next 11 bits are a number that represents the number of
		// sub-packets immediately contained by this packet.
		for (size_t i = 0; i < 11; i++) {
			eleven_bits.set(10 - i, this->transmission[pos++]);
		}
		unsigned long number_of_sub_packets = eleven_bits.to_ulong();

		unsigned long count = 0;
		while (count < number_of_sub_packets) {
			Packet sub_packet;
			processHeader(pos, sub_packet);
			processBody(pos, sub_packet);
			packet.values.push(evaluate(sub_packet));
			count++;
		}
	}

	void processSubpacketsByLength(size_t &pos, Packet &packet) {
		bitset<15> fiveteen_bits;

		// The next 15 bits are a number that represents the total length
		// in bits of the sub-packets contained by this packet.
		for (size_t i = 0; i < 15; i++) {
			fiveteen_bits.set(14 - i, this->transmission[pos++]);
		}
		unsigned long sub_packets_length = fiveteen_bits.to_ulong();
		unsigned long end_pos = pos + sub_packets_length;

		while (pos < end_pos) {
			Packet sub_packet;
			processHeader(pos, sub_packet);
			processBody(pos, sub_packet);
			packet.values.push(evaluate(sub_packet));
		}
	}

	void processHeader(size_t &pos, Packet &packet) {
		// Read version
		bitset<3> version_bits;
		version_bits.set(2, this->transmission[pos++]);
		version_bits.set(1, this->transmission[pos++]);
		version_bits.set(0, this->transmission[pos++]);
		packet.version = version_bits.to_ulong();

		// Read type_id
		bitset<3> type_id_bits;
		type_id_bits.set(2, this->transmission[pos++]);
		type_id_bits.set(1, this->transmission[pos++]);
		type_id_bits.set(0, this->transmission[pos++]);
		packet.type_id = type_id_bits.to_ulong();
	}

	void processBody(size_t &pos, Packet &packet) {
		if (packet.type_id == 4) {
			// Literal
			processLiteralValue(pos, packet);
		} else {
			// Process sub-packets
			bool fixed_mode = this->transmission[pos++];
			if (fixed_mode) {
				processSubpacketsByCount(pos, packet);
			} else {
				processSubpacketsByLength(pos, packet);
			}
		}
	}

	void processPacket(size_t &pos, Packet &packet) {
		processHeader(pos, packet);
		processBody(pos, packet);
	}

	public:
	void parse() {
		size_t pos = 0;
		Packet packet;

		processPacket(pos, packet);
		unsigned long answer = evaluate(packet);
		cout << "Answer is: " << to_string(answer) << endl;
	}

	void decode(const string &input) {
		size_t pos = 0;

		for (auto &c: input) {
			switch (c) {
				case '0':
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
				break;
				case '1':
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
				break;
				case '2':
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
				break;
				case '3':
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
				break;
				case '4':
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
				break;
				case '5':
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
				break;
				case '6':
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
				break;
				case '7':
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
				break;
				case '8':
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
				break;
				case '9':
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
				break;
				case 'A':
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
				break;
				case 'B':
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
				break;
				case 'C':
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, false);
				break;
				case 'D':
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
					this->transmission.set(pos++, true);
				break;
				case 'E':
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, false);
				break;
				case 'F':
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
					this->transmission.set(pos++, true);
				break;
			}
		}
		this->transmission_length = pos;
	}
};

class BuoyancyInterchangeTransmissionSystem {
	public:
	void receiveTransmission(const string &input) {
		Transmission transmission;
		transmission.decode(input);
		transmission.parse();
	}
};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day16b <input.txt>" << endl;
		return -1;
	}

	// Open & read file
	string path = argv[1];
	string input;
	ifstream file(path);
	getline(file, input);

	// And go!
	BuoyancyInterchangeTransmissionSystem bits;
	bits.receiveTransmission(input);

	return 0;
}
