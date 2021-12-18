#include <bitset>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

static const size_t MAX_TRANSMISSION_BITS = 10000;

struct Packet {
	unsigned long version;
	unsigned long type_id;
};

class Transmission {
	bitset<MAX_TRANSMISSION_BITS> transmission;
	size_t transmission_length = 0;
	unsigned long version_sum = 0;

	unsigned long processLiteralValue(size_t &pos) {
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

		cout << "Found literal: " << to_string(literal) << endl;
		return literal;
	}

	void processSubpacketsByCount(size_t &pos) {
		bitset<11> eleven_bits;

		// The next 11 bits are a number that represents the number of
		// sub-packets immediately contained by this packet.
		for (size_t i = 0; i < 11; i++) {
			eleven_bits.set(10 - i, this->transmission[pos++]);
		}
		unsigned long number_of_sub_packets = eleven_bits.to_ulong();

		cout << "Process " << to_string(number_of_sub_packets) << " sub-packet(s)" << endl;

		unsigned long count = 0;
		while (count < number_of_sub_packets) {
			cout << "   Processing sub-package " << to_string(count + 1);
			cout << " of " << to_string(number_of_sub_packets) << endl;
			Packet packet = processHeader(pos);
			this->version_sum += packet.version;
			processBody(pos, packet);
			count++;
		}
	}

	void processSubpacketsByLength(size_t &pos) {
		bitset<15> fiveteen_bits;

		// The next 15 bits are a number that represents the total length
		// in bits of the sub-packets contained by this packet.
		for (size_t i = 0; i < 15; i++) {
			fiveteen_bits.set(14 - i, this->transmission[pos++]);
		}
		unsigned long sub_packets_length = fiveteen_bits.to_ulong();
		unsigned long end_pos = pos + sub_packets_length;

		cout << "Process variable number of sub-packets (total size is ";
		cout << to_string(sub_packets_length) << ')' << endl;

		unsigned long count = 0;
		while (pos < end_pos) {
			cout << "   Processing sub-package " << to_string(count + 1) << endl;
			Packet packet = processHeader(pos);
			this->version_sum += packet.version;
			processBody(pos, packet);
		}
	}

	Packet processHeader(size_t &pos) {
		Packet packet;

		cout << "Processing header" << endl;

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

		cout << "  Version is " << to_string(packet.version);
		cout << ", type is " << to_string(packet.type_id) << endl;

		return packet;
	}

	void processBody(size_t &pos, Packet &packet) {
		if (packet.type_id == 4) {
			// Literal value
			processLiteralValue(pos);
		} else {
			// Operator packet
			bool fixed_mode = this->transmission[pos++];
			if (fixed_mode) {
				processSubpacketsByCount(pos);
			} else {
				processSubpacketsByLength(pos);
			}
		}
	}

	public:
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

	void parse() {
		size_t pos = 0;

		Packet packet = processHeader(pos);
		this->version_sum += packet.version;
		processBody(pos, packet);
	}

	unsigned long getVersionSum() const {
		return this->version_sum;
	}
};

class BuoyancyInterchangeTransmissionSystem {
	unsigned long version_sum = 0;

	public:
	void receiveTransmission(const string &input) {
		Transmission transmission;
		transmission.decode(input);
		transmission.parse();
		this->version_sum = transmission.getVersionSum();
	}

	unsigned long getAnswer() const {
		return this->version_sum;
	}
};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day16a <input.txt>" << endl;
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
	unsigned long answer = bits.getAnswer();
	cout << "Answer is " << to_string(answer) << endl;

	return 0;
}
