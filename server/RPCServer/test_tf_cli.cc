// linear_model_client.cpp

#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "../../proto-src/pred.grpc.pb.h"
#include "../../proto-src/pred.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using tensorflow::serving::PredictRequest;
using tensorflow::serving::PredictResponse;
using tensorflow::serving::PredictionService;

class TensorFlowServingClient {
public:
    TensorFlowServingClient(std::shared_ptr<Channel> channel)
            : stub_(PredictionService::NewStub(channel)) {}

    float Predict(float input) {
        PredictRequest request;
        PredictResponse response;
        ClientContext context;

        request.mutable_model_spec()->set_name("linear_model");
        request.mutable_model_spec()->set_signature_name("predict");

        tensorflow::TensorProto tensor_proto;
        tensor_proto.set_dtype(tensorflow::DataType::DT_FLOAT);
        tensor_proto.add_float_val(input);
        tensor_proto.mutable_tensor_shape()->add_dim()->set_size(1);

        (*request.mutable_inputs())["x"] = tensor_proto;

        Status status = stub_->Predict(&context, request, &response);

        if (!status.ok()) {
            std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
            return -1.0;
        }

        const auto& outputs = response.outputs();
        const auto& result_tensor = outputs.at("output_0");
        return result_tensor.float_val(0);
    }

private:
    std::unique_ptr<PredictionService::Stub> stub_;
};

int main(int argc, char** argv) {
    TensorFlowServingClient client(grpc::CreateChannel("localhost:8500", grpc::InsecureChannelCredentials()));
    float input = 5.0;  // Example input value
    float result = client.Predict(input);
    std::cout << "Prediction for input " << input << " is " << result << std::endl;
    return 0;
}
