import matplotlib.pyplot as plt


class BaseSave:
    def __init__(self, name):
        self.file_name = name
        self.base_time = []
        self.base_RA = []
        self.base_DEC = []
        self.barycentric_x = []
        self.barycentric_y = []
        self.barycentric_z = []

    def read_data(self):
        base_measure = open(self.file_name, 'r')
        if not base_measure:
            raise "Error of reading file"
        for line in base_measure:
            if line == ("\n" or " \n"):
                continue
            list_of_input_string = line.split()
            for i in range(len(list_of_input_string)):
                if i == 0:
                    self.base_time.append(float(list_of_input_string[i]))
                else:
                    if list_of_input_string[i] == "RA=":
                        self.base_RA.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "DEC=":
                        self.base_DEC.append(float(list_of_input_string[i + 1]))
        base_measure.close()

    def read_barycentric_data(self):
        barycentric_measure = open(self.file_name, "r")
        if not barycentric_measure:
            raise "Error of reading file"
        for line in barycentric_measure:
            if line == ("\n" or " \n"):
                continue
            list_of_input_string = line.split()
            for i in range(len(list_of_input_string)):
                if i == 0:
                    self.base_time.append(float(list_of_input_string[i]))
                else:
                    if list_of_input_string[i] == "x=":
                        self.barycentric_x.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "y=":
                        self.barycentric_y.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "z=":
                        self.barycentric_z.append(float(list_of_input_string[i + 1]))
        barycentric_measure.close()

    def print_data(self):
        print("time[", len(self.base_time), "]: ", self.base_time)
        print("RA[", len(self.base_RA), "]: ", self.base_RA)
        print("DEC[", len(self.base_DEC), "]: ", self.base_DEC)


class Model:
    def __init__(self, name):
        self.file_name = name
        self.base_time = []
        self.base_RA = []
        self.base_DEC = []
        self.barycentric_x = []
        self.barycentric_y = []
        self.barycentric_z = []

    def read_data(self):
        base_measure = open(self.file_name, 'r')
        if not base_measure:
            raise "Error of reading file"
        for line in base_measure:
            if line == ("\n" or " \n"):
                continue

            list_of_input_string = line.split()
            self.base_RA.append(float(list_of_input_string[0]))
            self.base_DEC.append(float(list_of_input_string[1]))
        base_measure.close()

    def print_data(self):
        print("time[", len(self.base_time), "]: ", self.base_time)
        print("RA[", len(self.base_RA), "]: ", self.base_RA)
        print("DEC[", len(self.base_DEC), "]: ", self.base_DEC)

def draw_2D_graphics(title, time, measure1, measure2, label1, label2, output_file):
    plt.figure()
    plt.grid()
    plt.title(title)
    plt.plot(time, measure1, label=label1)
    plt.plot(time, measure2, label=label2)

    plt.xlabel('time, MJD')
    plt.ylabel('Position, radians')
    plt.legend(loc='best')
    # plt.show()
    plt.savefig(output_file)
    plt.close()


def draw_2D_graphic_one_param(title, time, measure1, label1, output_file):
    plt.figure()
    plt.grid()
    plt.title(title)
    plt.scatter(time, measure1, label=label1)
    # plt.plot(time, measure2, label=label2)

    plt.xlabel('time, MJD')
    plt.ylabel('Position, radians')
    plt.legend(loc='best')
    # plt.show()
    plt.savefig(output_file)
    plt.close()


if __name__ == '__main__':
    base = BaseSave("../output_data/base_measure.txt")
    base.read_data()

    model_before = Model("../output_data/before_mnk.txt")
    model_before.read_data()
    model_after = Model("../output_data/after_mnk.txt")
    model_after.read_data()

    model_after.base_time = base.base_time.copy()
    model_before.base_time = base.base_time.copy()

    draw_2D_graphics("RA delta before and RA delta after", base.base_time, model_before.base_RA, model_after.base_RA,
                     "Before", "After", "./RA_BA")

    draw_2D_graphics("DEC delta before and DEC delta after", base.base_time, model_before.base_DEC, model_after.base_DEC,
                     "Before", "After", "./DEC_BA")

