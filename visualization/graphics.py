import matplotlib.pyplot as plt


class BaseSave:
    def __init__(self, name):
        self.file_name = name
        self.base_time = []
        self.base_long = []
        self.base_lati = []

    def read_data(self):
        base_measure = open(self.file_name, 'r')
        if not base_measure:
            raise "Error of reading file"
        self.base_time = []
        self.base_long = []
        self.base_lati = []
        for line in base_measure:
            if line == ("\n" or " \n"):
                continue
            list_of_input_string = line.split()
            for i in range(len(list_of_input_string)):
                if i == 0:
                    self.base_time.append(float(list_of_input_string[i]))
                else:
                    if list_of_input_string[i] == "long=":
                        self.base_long.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "lati=":
                        self.base_lati.append(float(list_of_input_string[i + 1]))
        base_measure.close()

    def print_readed(self):
        print("time[", len(self.base_time), "]: ", self.base_time)
        print("long[", len(self.base_long), "]: ", self.base_long)
        print("lati[", len(self.base_lati), "]: ", self.base_lati)


class ModelSave:
    def __init__(self, name):
        self.file_name = name
        self.model_time = []
        self.model_long = []
        self.model_lati = []
        self.model_vx = []
        self.model_vy = []
        self.model_vz = []

    def read_data(self):
        model_measure = open(self.file_name, 'r')  # put path here
        if not model_measure:
            raise "Error of reading file"

        for line in model_measure:
            if line == ("\n" or " \n"):
                continue
            list_of_input_string = line.split()
            for i in range(len(list_of_input_string)):
                if i == 0:
                    self.model_time.append(float(list_of_input_string[i]))
                else:
                    if list_of_input_string[i] == "long=":
                        self.model_long.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "lati=":
                        self.model_lati.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "vx(km/s)=":
                        self.model_vx.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "vy(km/s)=":
                        self.model_vy.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "vz(km/s)=":
                        self.model_vz.append(float(list_of_input_string[i + 1]))

        model_measure.close()

    def print_readed(self):
        print("time[", len(self.model_time), "]: ", self.model_time)
        print("long[", len(self.model_long), "]: ", self.model_long)
        print("lati[", len(self.model_lati), "]: ", self.model_lati)

        print("vx[", len(self.model_vx), "]: ", self.model_vx)
        print("vy[", len(self.model_vy), "]: ", self.model_vy)
        print("vz[", len(self.model_vz), "]: ", self.model_vz)


def draw_2D_graphics(title, time, measure1, measure2, label1, label2, output_file):
    plt.figure()
    plt.grid()
    plt.title(title)
    plt.plot(time, measure1, label=label1)
    plt.plot(time, measure2, label=label2)

    plt.xlabel('time, MJD')
    plt.ylabel('Position, degrees')
    plt.legend(loc='best')
    # plt.show()
    plt.savefig(output_file)
    plt.close()


if __name__ == '__main__':
    base = BaseSave("../output_data/base_measure.txt")
    base.read_data()
    model = ModelSave("../output_data/model_measure.txt")
    model.read_data()

    draw_2D_graphics('Latitude in compare', model.model_time, base.base_lati, model.model_lati,
                     'Base', 'Model', './lati_compare')
    draw_2D_graphics('Longitude in compare', model.model_time, base.base_long, model.model_long,
                     'Base', 'Model', './long_compare')
