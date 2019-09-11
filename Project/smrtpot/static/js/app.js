function getTimeNow() {
    var now = new Date();
    var h = now.getHours(),
        m = now.getMinutes(),
        s = now.getSeconds();

    // if single digit values, add padding
    if (h < 10) {
        // pad number
        h = "0" + h;
    }

    if (m < 10) {
        m = "0" + m;
    }

    if (s < 10) {
        s = "0" + s;
    }

    return [h, m, s].join(":");
}

// Update bootstrap dropdown button label/values based off user selection
$(function(){
    $(".dropdown-menu#day-select a").click(function() {
        $("#day-select.btn:first-child").text($(this).text());
        $("#day-select.btn:first-child").val($(this).text());
    });

    $(".dropdown-menu#plant-type-select a").click(function() {
        $("#plant-type-select.btn:first-child").text($(this).text());
        $("#plant-type-select.btn:first-child").val($(this).text());
    });

    $(".dropdown-menu#schedule-select a").click(function() {
        // next two lines are a bit redundant since handled by flask template vars
        // leaving here in case the refreshing issue gets fixed
        $(".btn[name='selectedScheduleLabel']:first-child").text($(this).text());
        $(".btn[name='selectedScheduleLabel']:first-child").val($(this).text());
        $("input[name='selectedSchedule']").val($(this).text());
        $("#select-schedule").submit();

        /* Attempts to use ajax in hopes of not having to
            refresh entire page when loading different schedules (rip)
        */
        // send selected schedule to server side
        // $.getJSON('/schedules', {
        //   scheduleName: $(this).text()
        // }, function(data) {
        //     console.log("data: ", data);
        // });

        // $.getJSON("/", {scheduleName: $(this).text()}, function(json) {
        //   // use json here
        // }).done(function(){
        //     window.location.href = '/';
        // });
        // return false; // this will disable default behavior
    });
});
