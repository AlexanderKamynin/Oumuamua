import matplotlib.pyplot as plt


class BaseSave:
    def __init__(self, name):
        self.file_name = name
        self.base_time = []
        self.base_RA = []
        self.base_DEC = []

    def read_data(self):
        base_measure = open(self.file_name, 'r')
        if not base_measure:
            raise "Error of reading file"
        self.base_time = []
        self.base_RA = []
        self.base_DEC = []
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

    def print_readed(self):
        print("time[", len(self.base_time), "]: ", self.base_time)
        print("RA[", len(self.base_RA), "]: ", self.base_RA)
        print("DEC[", len(self.base_DEC), "]: ", self.base_DEC)


class ModelSave:
    def __init__(self, name):
        self.file_name = name
        self.model_time = []
        self.model_RA = []
        self.model_DEC = []
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
                    if list_of_input_string[i] == "RA=":
                        self.model_RA.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "DEC=":
                        self.model_DEC.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "vx(km/s)=":
                        self.model_vx.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "vy(km/s)=":
                        self.model_vy.append(float(list_of_input_string[i + 1]))
                    if list_of_input_string[i] == "vz(km/s)=":
                        self.model_vz.append(float(list_of_input_string[i + 1]))

        model_measure.close()

    def print_readed(self):
        print("time[", len(self.model_time), "]: ", self.model_time)
        print("RA[", len(self.model_RA), "]: ", self.model_RA)
        print("DEC[", len(self.model_DEC), "]: ", self.model_DEC)

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
    plt.ylabel('Position, radians')
    plt.legend(loc='best')
    # plt.show()
    plt.savefig(output_file)
    plt.close()
    
    
def draw_2D_graphic_one_param(title, time, measure1,  label1,  output_file):
    plt.figure()
    plt.grid()
    plt.title(title)
    plt.scatter(time, measure1, label=label1)
    #plt.plot(time, measure2, label=label2)

    plt.xlabel('time, MJD')
    plt.ylabel('Position, radians')
    plt.legend(loc='best')
    # plt.show()
    plt.savefig(output_file)
    plt.close()

if __name__ == '__main__':
    base = BaseSave("../output_data/base_measure.txt")
    base.read_data()
    model = ModelSave("../output_data/model_measure.txt")
    model.read_data()

    draw_2D_graphics('Right ascension in compare', model.model_time, base.base_RA, model.model_RA,
                     'Base', 'Model', './RA_compare')
    draw_2D_graphics('Declination in compare', model.model_time, base.base_DEC, model.model_DEC,
                     'Base', 'Model', './DEC_compare')
    
    
    delta_RA_list = [] # base - model
    delta_DEC_list = [] # base - model
    for i in range(len(model.model_RA)):
        delta_RA_list.append(abs(model.model_RA[i] - base.base_RA[i]))
        delta_DEC_list.append(abs(model.model_DEC[i] - base.base_DEC[i]))
    draw_2D_graphic_one_param("Base-Model delta [RA]", model.model_time, delta_RA_list, "RA delta", "./RA_delta")
    draw_2D_graphic_one_param("Base-Model delta [DEC]", model.model_time, delta_DEC_list, "DEC delta","./DEC_delta")
