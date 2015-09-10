all: noisy_channel stop-wait-receiver stop-wait-sender go-back-n-sender go-back-n-receiver selective-repeat-sender selective-repeat-receiver

clean:
	rm -f channel_l_in channel_r_in channel_l_out channel_r_out
